
from iocbuilder import Device
from iocbuilder.arginfo import *
from iocbuilder.modules.asyn import Asyn, AsynIP
from iocbuilder import AutoSubstitution


class _rgamv2Template(AutoSubstitution):
    TemplateFile = 'rgamv2.template'

class rgamv2(Device):
    # this uses asyn
    Dependencies = (Asyn,)

    LibFileList = ['rgamv2']
    DbdFileList = ['rgamv2Include']

    # simple init, just store the arguments
    def __init__(self, name, ipPort, **args):
        self.__super.__init__()
        self.name = name
        self.ipPort = ipPort
        args["PORT"] = name
        args["name"] = name
        self.template = _rgamv2Template(**filter_dict(args, _rgamv2Template.ArgInfo.Names()))

    # this will appear once per instantiation
    def Initialise(self):
        print 'mv2init("%(name)s", "%(ipPort)s")' % self.__dict__

    # tell xmlbuilder what args to supply
    ArgInfo = _rgamv2Template.ArgInfo.filtered(without=["PORT"]) + makeArgInfo(__init__,
        name    = Simple("Name of asyn port to create", str),
        ipPort  = Ident("IP port object", AsynIP))



