/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * pvAccessCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <pv/serverContext.h>
#include <pv/responseHandlers.h>
#include <epicsSignal.h>

#include <pv/logger.h>

using namespace std;
using namespace epics::pvData;
using std::tr1::dynamic_pointer_cast;
using std::tr1::static_pointer_cast;

namespace epics { namespace pvAccess {

const char* ServerContextImpl::StateNames[] = { "NOT_INITIALIZED", "INITIALIZED", "RUNNING", "SHUTDOWN", "DESTROYED"};
const Version ServerContextImpl::VERSION("pvAccess Server", "cpp", 1, 2, 0, true);

ServerContextImpl::ServerContextImpl():
				_state(NOT_INITIALIZED),
				_beaconAddressList(),
				_ignoreAddressList(),
				_autoBeaconAddressList(true),
				_beaconPeriod(15.0),
				_broadcastPort(PVA_BROADCAST_PORT),
				_serverPort(PVA_SERVER_PORT),
				_receiveBufferSize(MAX_TCP_RECV),
				_timer(),
				_broadcastTransport(),
				_beaconEmitter(),
				_acceptor(),
				_transportRegistry(),
				_channelAccess(),
				_channelProviderNames(PVACCESS_DEFAULT_PROVIDER),
				_channelProviders(),
				_beaconServerStatusProvider()

{
    // TODO maybe there is a better place for this (when there will be some factory)
    epicsSignalInstallSigAlarmIgnore ();
    epicsSignalInstallSigPipeIgnore ();

	initializeLogger();
	loadConfiguration();
}

ServerContextImpl::shared_pointer ServerContextImpl::create()
{
    ServerContextImpl::shared_pointer thisPointer(new ServerContextImpl());
    return thisPointer;
}

ServerContextImpl::~ServerContextImpl()
{
    dispose();
}

const Version& ServerContextImpl::getVersion()
{
    return ServerContextImpl::VERSION;
}

void ServerContextImpl::initializeLogger()
{
	//createFileLogger("serverContextImpl.log");
}

struct noop_deleter
{
    template<class T> void operator()(T * p) {}
};

Configuration::shared_pointer ServerContextImpl::getConfiguration()
{
	Lock guard(_mutex);
	if (configuration.get() == 0)
	{
		ConfigurationProvider::shared_pointer configurationProvider = ConfigurationFactory::getProvider();
		configuration = configurationProvider->getConfiguration("pvAccess-server");
		if (configuration.get() == 0)
		{
			configuration = configurationProvider->getConfiguration("system");
		}
	}
	return configuration;
}

/**
 * Load configuration.
 */
void ServerContextImpl::loadConfiguration()
{
	Configuration::shared_pointer config = getConfiguration();

    _beaconAddressList = config->getPropertyAsString("EPICS_PVA_ADDR_LIST", _beaconAddressList);
    _beaconAddressList = config->getPropertyAsString("EPICS_PVAS_BEACON_ADDR_LIST", _beaconAddressList);

    _autoBeaconAddressList = config->getPropertyAsBoolean("EPICS_PVA_AUTO_ADDR_LIST", _autoBeaconAddressList);
    _autoBeaconAddressList = config->getPropertyAsBoolean("EPICS_PVAS_AUTO_BEACON_ADDR_LIST", _autoBeaconAddressList);

    _beaconPeriod = config->getPropertyAsFloat("EPICS_PVA_BEACON_PERIOD", _beaconPeriod);
    _beaconPeriod = config->getPropertyAsFloat("EPICS_PVAS_BEACON_PERIOD", _beaconPeriod);

    _serverPort = config->getPropertyAsInteger("EPICS_PVA_SERVER_PORT", _serverPort);
    _serverPort = config->getPropertyAsInteger("EPICS_PVAS_SERVER_PORT", _serverPort);

    _broadcastPort = config->getPropertyAsInteger("EPICS_PVA_BROADCAST_PORT", _broadcastPort);
    _broadcastPort = config->getPropertyAsInteger("EPICS_PVAS_BROADCAST_PORT", _broadcastPort);

    _receiveBufferSize = config->getPropertyAsInteger("EPICS_PVA_MAX_ARRAY_BYTES", _receiveBufferSize);
    _receiveBufferSize = config->getPropertyAsInteger("EPICS_PVAS_MAX_ARRAY_BYTES", _receiveBufferSize);

    _channelProviderNames = config->getPropertyAsString("EPICS_PVA_PROVIDER_NAMES", _channelProviderNames);
    _channelProviderNames = config->getPropertyAsString("EPICS_PVAS_PROVIDER_NAMES", _channelProviderNames);
}

void ServerContextImpl::initialize(ChannelAccess::shared_pointer const & channelAccess)
{
	Lock guard(_mutex);
	if (channelAccess == NULL)
	{
		THROW_BASE_EXCEPTION("non null channelAccess expected");
	}

	if (_state == DESTROYED)
	{
		THROW_BASE_EXCEPTION("Context destroyed.");
	}
	else if (_state != NOT_INITIALIZED)
	{
		THROW_BASE_EXCEPTION("Context already initialized.");
	}

	_channelAccess = channelAccess;


    // split comma separated names
    std::stringstream ss(_channelProviderNames);
    std::string providerName;
    while (std::getline(ss, providerName, ',')) {
    	ChannelProvider::shared_pointer channelProvider = _channelAccess->getProvider(providerName);
    	if (channelProvider)
            _channelProviders.push_back(channelProvider);
    }    

	//_channelProvider = _channelAccess->getProvider(_channelProviderNames);
	if (_channelProviders.size() == 0)
	{
		std::string msg = "None of the specified channel providers are available: " + _channelProviderNames + ".";
		THROW_BASE_EXCEPTION(msg.c_str());
	}

	internalInitialize();

	_state = INITIALIZED;
}

std::auto_ptr<ResponseHandler> ServerContextImpl::createResponseHandler()
{
    ServerContextImpl::shared_pointer thisContext = shared_from_this();
    return std::auto_ptr<ResponseHandler>(new ServerResponseHandler(thisContext));    
}

void ServerContextImpl::internalInitialize()
{
    osiSockAttach();

	_timer.reset(new Timer("pvAccess-server timer", lowerPriority));
	_transportRegistry.reset(new TransportRegistry());

	// setup broadcast UDP transport
	initializeBroadcastTransport();

    ServerContextImpl::shared_pointer thisServerContext = shared_from_this();

	_acceptor.reset(new BlockingTCPAcceptor(thisServerContext, thisServerContext, _serverPort, _receiveBufferSize));
	_serverPort = ntohs(_acceptor->getBindAddress()->ia.sin_port);

	_beaconEmitter.reset(new BeaconEmitter(_broadcastTransport, thisServerContext));
}

void ServerContextImpl::initializeBroadcastTransport()
{
	// setup UDP transport
	try
	{
		// where to bind (listen) address
	    osiSockAddr listenLocalAddress;
	    listenLocalAddress.ia.sin_family = AF_INET;
	    listenLocalAddress.ia.sin_port = htons(_broadcastPort);
	    listenLocalAddress.ia.sin_addr.s_addr = htonl(INADDR_ANY);

		// where to send addresses
	    SOCKET socket = epicsSocketCreate(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	    if (socket == INVALID_SOCKET)
	    {
	    	THROW_BASE_EXCEPTION("Failed to initialize broadcast UDP transport");
	    }
	    auto_ptr<InetAddrVector> broadcastAddresses(getBroadcastAddresses(socket,_broadcastPort));
	    epicsSocketDestroy(socket);

        TransportClient::shared_pointer nullTransportClient;

	    auto_ptr<BlockingUDPConnector> broadcastConnector(new BlockingUDPConnector(true, true));
	    auto_ptr<epics::pvAccess::ResponseHandler> responseHandler = createResponseHandler();
        _broadcastTransport = static_pointer_cast<BlockingUDPTransport>(broadcastConnector->connect(
                nullTransportClient, responseHandler,
                listenLocalAddress, PVA_PROTOCOL_REVISION,
                PVA_DEFAULT_PRIORITY));
		_broadcastTransport->setBroadcastAddresses(broadcastAddresses.get());

		// set ignore address list
		if (!_ignoreAddressList.empty())
		{
			// we do not care about the port
			auto_ptr<InetAddrVector> list(getSocketAddressList(_ignoreAddressList, 0, NULL));
			if (list.get() != NULL && list->size() > 0)
			{
				_broadcastTransport->setIgnoredAddresses(list.get());
			}
		}
		// set broadcast address list
		if (!_beaconAddressList.empty())
		{
			// if auto is true, add it to specified list
			InetAddrVector* appendList = NULL;
			if (_autoBeaconAddressList == true)
			{
				appendList = _broadcastTransport->getSendAddresses();
			}

			auto_ptr<InetAddrVector> list(getSocketAddressList(_beaconAddressList, _broadcastPort, appendList));
			if (list.get() != NULL  && list->size() > 0)
			{
				_broadcastTransport->setBroadcastAddresses(list.get());
			}
		}

		_broadcastTransport->start();
	}
	catch (std::exception& e)
	{
		THROW_BASE_EXCEPTION_CAUSE("Failed to initialize broadcast UDP transport", e);
	}
	catch (...)
	{
		THROW_BASE_EXCEPTION("Failed to initialize broadcast UDP transport");
	}
}

void ServerContextImpl::run(int32 seconds)
{
	if (seconds < 0)
	{
		THROW_BASE_EXCEPTION("seconds cannot be negative.");
	}

	{
		Lock guard(_mutex);

		if (_state == NOT_INITIALIZED)
		{
			THROW_BASE_EXCEPTION("Context not initialized.");
		}
		else if (_state == DESTROYED)
		{
			THROW_BASE_EXCEPTION("Context destroyed.");
		}
		else if (_state == RUNNING)
		{
			THROW_BASE_EXCEPTION("Context is already running.");
		}
		else if (_state == SHUTDOWN)
		{
			THROW_BASE_EXCEPTION("Context was shutdown.");
		}

		_state = RUNNING;
	}

	// run...
	_beaconEmitter->start();

	//TODO review this
	if(seconds == 0)
	{
		_runEvent.wait();
	}
	else
	{
		_runEvent.wait(seconds);
	}

	{
		Lock guard(_mutex);
		_state = SHUTDOWN;
	}
}

void ServerContextImpl::shutdown()
{
	Lock guard(_mutex);
	if(_state == DESTROYED)
	{
		THROW_BASE_EXCEPTION("Context already destroyed.");
	}

	// notify to stop running...
	_runEvent.signal();
}

void ServerContextImpl::destroy()
{
	Lock guard(_mutex);
	if (_state == DESTROYED)
	{
		// silent return
		return;
		// exception is not OK, since we use
		// shared_pointer-s auto-cleanup/destruction
		// THROW_BASE_EXCEPTION("Context already destroyed.");
	}

	// shutdown if not already
	shutdown();

	// go into destroyed state ASAP
	_state = DESTROYED;

	internalDestroy();
}


void ServerContextImpl::internalDestroy()
{
	// stop responding to search requests
	if (_broadcastTransport != NULL)
	{
		_broadcastTransport->close();
		_broadcastTransport.reset();
	}

	// stop accepting connections
	if (_acceptor != NULL)
	{
		_acceptor->destroy();
		_acceptor.reset();
	}

	// stop emitting beacons
	if (_beaconEmitter != NULL)
	{
		_beaconEmitter->destroy();
		_beaconEmitter.reset();
	}

	// this will also destroy all channels
	destroyAllTransports();
}

void ServerContextImpl::destroyAllTransports()
{

	// not initialized yet
	if (_transportRegistry == NULL)
	{
		return;
	}

	std::auto_ptr<TransportRegistry::transportVector_t> transports = _transportRegistry->toArray();
	if (transports.get() == 0)
	   return;
	   
    int size = (int)transports->size();
	if (size == 0)
		return;

	LOG(logLevelInfo, "Server context still has %d transport(s) active and closing...", size);

	for (int i = 0; i < size; i++)
	{
		Transport::shared_pointer transport = (*transports)[i];
		try
		{
			transport->close();
		}
		catch (std::exception &e)
		{
			// do all exception safe, log in case of an error
			LOG(logLevelError, "Unhandled exception caught from client code at %s:%d: %s", __FILE__, __LINE__, e.what());
		}
		catch (...)
		{
			// do all exception safe, log in case of an error
			 LOG(logLevelError, "Unhandled exception caught from client code at %s:%d.", __FILE__, __LINE__);
		}
	}
	
	// now clear all (release)
	_transportRegistry->clear();
	
}

void ServerContextImpl::printInfo()
{
	printInfo(cout);
}

void ServerContextImpl::printInfo(ostream& str)
{
	Lock guard(_mutex);
	str << "VERSION : " << getVersion().getVersionString() << endl \
		<< "PROVIDER_NAMES : " << _channelProviderNames << endl \
		<< "BEACON_ADDR_LIST : " << _beaconAddressList << endl \
	    << "AUTO_BEACON_ADDR_LIST : " << _autoBeaconAddressList << endl \
	    << "BEACON_PERIOD : " << _beaconPeriod << endl \
	    << "BROADCAST_PORT : " << _broadcastPort << endl \
	    << "SERVER_PORT : " << _serverPort << endl \
	    << "RCV_BUFFER_SIZE : " << _receiveBufferSize << endl \
	    << "IGNORE_ADDR_LIST: " << _ignoreAddressList << endl \
	    << "STATE : " << ServerContextImpl::StateNames[_state] << endl;
}

void ServerContextImpl::dispose()
{
	try
	{
		destroy();
	}
	catch(...)
	{
		// noop
	}
}

void ServerContextImpl::setBeaconServerStatusProvider(BeaconServerStatusProvider::shared_pointer const & beaconServerStatusProvider)
{
	_beaconServerStatusProvider = beaconServerStatusProvider;
}

bool ServerContextImpl::isInitialized()
{
	Lock guard(_mutex);
	return _state == INITIALIZED || _state == RUNNING || _state == SHUTDOWN;
}

bool ServerContextImpl::isDestroyed()
{
	Lock guard(_mutex);
	return _state == DESTROYED;
}

std::string ServerContextImpl::getBeaconAddressList()
{
	return _beaconAddressList;
}

bool ServerContextImpl::isAutoBeaconAddressList()
{
	return _autoBeaconAddressList;
}

float ServerContextImpl::getBeaconPeriod()
{
	return _beaconPeriod;
}

int32 ServerContextImpl::getReceiveBufferSize()
{
	return _receiveBufferSize;
}

int32 ServerContextImpl::getServerPort()
{
	return _serverPort;
}

void ServerContextImpl::setServerPort(int32 port)
{
	_serverPort = port;
}

int32 ServerContextImpl::getBroadcastPort()
{
	return _broadcastPort;
}

std::string ServerContextImpl::getIgnoreAddressList()
{
	return _ignoreAddressList;
}

BeaconServerStatusProvider::shared_pointer ServerContextImpl::getBeaconServerStatusProvider()
{
	return _beaconServerStatusProvider;
}

osiSockAddr* ServerContextImpl::getServerInetAddress()
{
	if(_acceptor != NULL)
	{
		return const_cast<osiSockAddr*>(_acceptor->getBindAddress());
	}
	return NULL;
}

BlockingUDPTransport::shared_pointer ServerContextImpl::getBroadcastTransport()
{
	return _broadcastTransport;
}

ChannelAccess::shared_pointer ServerContextImpl::getChannelAccess()
{
	return _channelAccess;
}

std::string ServerContextImpl::getChannelProviderName()
{
	return _channelProviderNames;
}

// NOTE: not synced
void ServerContextImpl::setChannelProviderName(std::string channelProviderName)
{
    if (_state != NOT_INITIALIZED)
        throw std::logic_error("must be called before initialize");
    _channelProviderNames = channelProviderName;
}

std::vector<ChannelProvider::shared_pointer> ServerContextImpl::getChannelProviders()
{
	return _channelProviders;
}

Timer::shared_pointer ServerContextImpl::getTimer()
{
	return _timer;
}

TransportRegistry::shared_pointer ServerContextImpl::getTransportRegistry()
{
	return _transportRegistry;
}

Channel::shared_pointer ServerContextImpl::getChannel(pvAccessID /*id*/)
{
	// not used
	return Channel::shared_pointer();
}

Transport::shared_pointer ServerContextImpl::getSearchTransport()
{
	// not used 
	return Transport::shared_pointer();
}

void ServerContextImpl::newServerDetected()
{
    // not used
}


}
}
