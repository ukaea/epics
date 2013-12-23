/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * pvAccessCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#ifndef BEACONEMITTER_H
#define BEACONEMITTER_H

#include <pv/timer.h>
#include <pv/timeStamp.h>
#include <pv/remote.h>

#include <osiSock.h>

#include <pv/beaconServerStatusProvider.h>
//#include <pv/serverContext.h>
#include <pv/sharedPtr.h>

namespace epics { namespace pvAccess {

    class ServerContextImpl;

	/**
	 * BeaconEmitter
	 *
	 * @author gjansa
	 */
	class BeaconEmitter:
	   public TransportSender,
	   public epics::pvData::TimerCallback,
	   public std::tr1::enable_shared_from_this<BeaconEmitter>
	{
	public:
        typedef std::tr1::shared_ptr<BeaconEmitter> shared_pointer;
        typedef std::tr1::shared_ptr<const BeaconEmitter> const_shared_pointer;

		/**
		 * Constructor.
		 * @param transport	transport to be used to send beacons.
		 * @param context PVA context.
		 */
//		BeaconEmitter(Transport::shared_pointer const & transport, ServerContextImpl::shared_pointer const & context);
		BeaconEmitter(Transport::shared_pointer const & transport, std::tr1::shared_ptr<ServerContextImpl>& context);
		
        /**
		 * Test Constructor (ohne context)
		 * @param transport	transport to be used to send beacons.
		 */
		BeaconEmitter(Transport::shared_pointer const & transport, const osiSockAddr& serverAddress);
        
		virtual ~BeaconEmitter();

		void lock();
		void unlock();
		
		void send(epics::pvData::ByteBuffer* buffer, TransportSendControl* control);

		void timerStopped();

		/**
		 * Start emitting.
		 */
		void start();
		
        /**
		 * Reschedule timer.
		 */
		void reschedule();
		
        /**
		 * Timer callback.
		 */
		void callback();

		void destroy();
	
    private:

		/**
		 * Minimal (initial) PVA beacon period (in seconds).
		 */
		static const float EPICS_PVA_MIN_BEACON_PERIOD;

		/**
		 * Minimal PVA beacon count limit.
		 */
		static const float EPICS_PVA_MIN_BEACON_COUNT_LIMIT;

		/**
		 * Transport.
		 */
        Transport::shared_pointer _transport;

		/**
		 * Beacon sequence ID.
		 */
		epics::pvData::int16 _beaconSequenceID;

		/**
		 * Startup timestamp (when clients detect a change, they will consider server restarted).
		 */
		epics::pvData::TimeStamp _startupTime;

		/**
		 * Fast (at startup) beacon period (in sec).
		 */
		double _fastBeaconPeriod;

		/**
		 * Slow (after beaconCountLimit is reached) beacon period (in sec).
		 */
		double _slowBeaconPeriod;

		/**
		 * Limit on number of beacons issued.
		 */
		epics::pvData::int16 _beaconCountLimit;

		/**
		 * Server address.
		 */
		const osiSockAddr _serverAddress;

		/**
		 * Server port.
		 */
		epics::pvData::int32 _serverPort;

		/**
		 * Server status provider implementation (optional).
		 */
        BeaconServerStatusProvider::shared_pointer _serverStatusProvider;

		/**
		 * Timer.
		 */
        epics::pvData::Timer::shared_pointer _timer;
	};

}}

#endif  /* BEACONEMITTER_H */