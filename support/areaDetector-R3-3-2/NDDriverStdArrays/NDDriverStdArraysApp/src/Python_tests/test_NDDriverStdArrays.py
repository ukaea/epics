import unittest
import epics
import numpy as np
import time
from collections import OrderedDict

P   = '13NDSA1:'
R   = 'cam1:'
I   = 'image1:'
ROI = 'ROI1:'
ND_ARRAY_MAX_DIMS = 10

dataTypes = OrderedDict ([
    ('int8'   , 0),
    ('uint8'  , 1),
    ('int16'  , 2),
    ('uint16' , 3),
    ('int32'  , 4),
    ('uint32' , 5),
    ('float32', 6),
    ('float64', 7)
])

npDataTypes = OrderedDict ([
    ('int8'   , np.int8),
    ('uint8'  , np.uint8),
    ('int16'  , np.int16),
    ('uint16' , np.uint16),
    ('int32'  , np.int32),
    ('uint32' , np.uint32),
    ('float32', np.float32),
    ('float64', np.float64)
])


def dist(nx=256,ny=None):
    ''' Implements the IDL dist() function in Python
	'''
    if ny is None:
        ny = nx
    x = np.linspace(start=-nx/2,stop=nx/2-1, num=nx)
    y = np.linspace(start=-ny/2,stop=ny/2-1, num=ny)
    xx = x + 1j * y[:,np.newaxis]
    out = np.roll(np.roll(np.abs(xx),nx/2,1),ny/2,0)
    return out

def shift(array=None, sx=0, sy=0, sz=0):
    ''' Implements the IDL shift() function in Python
    '''
    if array is None:
        return array
    ndim = len(array.shape)
    if ndim > 3:
        print 'Maximum number of dimensions for array is 3'
        return array
    s = [sx, sy, sz]
    for i in range(ndim):
        array = np.roll(array,s[i],ndim-i-1)
    return array

class TestNDDriverStdArrays(unittest.TestCase):
    
    def setUp(self):
        print('Begin setup')
        self.acquirePV           = epics.PV(P+R+'Acquire.VAL')
        self.arrayCallbacksPV    = epics.PV(P+R+'ArrayCallbacks.VAL')
        self.arrayInPV           = epics.PV(P+R+'ArrayIn')
        self.nDimensionsPV       = epics.PV(P+R+'NDimensions.VAL')
        self.dimensionsPV        = epics.PV(P+R+'Dimensions.VAL')
        self.dataTypePV          = epics.PV(P+R+'DataType.VAL')
        self.colorModePV         = epics.PV(P+R+'ColorMode.VAL')
        self.imageModePV         = epics.PV(P+R+'ImageMode.VAL')
        self.numImagesPV         = epics.PV(P+R+'NumImages.VAL')
        self.appendModePV        = epics.PV(P+R+'AppendMode.VAL')
        self.callbackModePV      = epics.PV(P+R+'CallbackMode.VAL')
        self.doCallbacksPV       = epics.PV(P+R+'DoCallbacks.VAL')
        self.newArrayPV          = epics.PV(P+R+'NewArray.VAL')
        self.arrayCompletePV     = epics.PV(P+R+'ArrayComplete.VAL')
        self.numElementsPV       = epics.PV(P+R+'NumElements_RBV.VAL')
        self.nextElementPV       = epics.PV(P+R+'NextElement.VAL')
        self.stridePV            = epics.PV(P+R+'Stride.VAL')
        self.fillValuePV         = epics.PV(P+R+'FillValue.VAL')
        self.imageArrayPV        = epics.PV(P+I+'ArrayData.VAL')
        self.imageEnablePV       = epics.PV(P+I+'EnableCallbacks.VAL')
        self.imagePortPV         = epics.PV(P+I+'NDArrayPort.VAL')
        self.ROIPortPV           = epics.PV(P+ROI+'NDArrayPort.VAL')
        self.ROIEnablePV         = epics.PV(P+ROI+'EnableCallbacks.VAL')
        self.ROICollapseDimsPV   = epics.PV(P+ROI+'CollapseDims.VAL')
        self.ROIStartPVs         = [epics.PV(P+ROI+'MinX.VAL'),
                                    epics.PV(P+ROI+'MinY.VAL'),
                                    epics.PV(P+ROI+'MinZ.VAL')]
        self.ROISizePVs          = [epics.PV(P+ROI+'SizeX.VAL'),
                                    epics.PV(P+ROI+'SizeY.VAL'),
                                    epics.PV(P+ROI+'SizeZ.VAL')]
        print('PVs connected')

        self.arrayCallbacksPV.put('Enable')
        self.ROIPortPV.put('NDSA')
        self.ROIEnablePV.put('Enable')
        self.imageEnablePV.put('Enable')
        print('Completed setup')

    def tearDown(self):
        pass
        
    def setupPVs(self, dimensions=[256, 512], dataType='Float64', 
        colorMode='Mono', imageMode='Single', numImages=1,
        appendMode='Disable', callbackMode='On update', 
        nextElement=0, stride=1, fillValue=0,
        roiStart=[0, 0, 0], roiSize=[10000, 10000, 10000],
        imagePort='NDSA'):
        
        self.nDimensionsPV.put(len(dimensions))
        for i in range(len(dimensions), ND_ARRAY_MAX_DIMS): dimensions.append(0)
        self.dimensionsPV.put(dimensions)
        self.dataTypePV.put(dataType)
        self.colorModePV.put(colorMode)
        self.imageModePV.put(imageMode)
        self.numImagesPV.put(numImages)
        self.appendModePV.put(appendMode)
        self.callbackModePV.put(callbackMode)
        self.nextElementPV.put(nextElement)
        self.fillValuePV.put(fillValue)
        self.stridePV.put(stride)
        for i in range(len(self.ROIStartPVs)):
          self.ROIStartPVs[i].put(roiStart[i])
          self.ROISizePVs[i].put(roiSize[i])
        self.imagePortPV.put(imagePort)

    def testDataTypes(self):
        # Test AppendMode=Disable
        nx, ny = 256, 768
        result = True
        scale = 100
        scaleStep = 1.05
        self.setupPVs(dimensions=[nx, ny])
        for dt in dataTypes:
            print 'testDataTypes type={:s}'.format(dt) 
            self.dataTypePV.put(dataTypes[dt])
            row = np.arange(nx) / (nx-1.) * scale
            scale *= scaleStep
            image = np.tile(row, ny).reshape(ny, nx).transpose()
            image = image.astype(npDataTypes[dt])
            self.acquirePV.put('Acquire')
            self.arrayInPV.put(image.flatten(order='F'), wait=True)
            rimage = self.imageArrayPV.get(count=nx*ny).reshape(nx,ny,order='F')
            result = result and self.assertTrue(np.array_equal(image, rimage))
        return result

    def testDataTypesAppend(self):
        # Test AppendMode=Enable 
        nx, ny = 256, 768
        result = True
        yStep = 1
        scale = 50
        scaleStep = 1.1
        self.setupPVs(dimensions=[nx, ny], appendMode='Enable')
        for dt in dataTypes:
            print 'testDataTypesAppend type={:s}'.format(dt) 
            yStart = 0
            self.newArrayPV.put(1)
            self.dataTypePV.put(dataTypes[dt])
            self.acquirePV.put('Acquire')
            row = np.arange(nx) / (nx-1.) * scale
            scale *= scaleStep
            image = np.tile(row, ny).reshape(ny, nx).transpose()
            image = image.astype(npDataTypes[dt])
            while yStart < ny:
              chunk = image[:, yStart:yStart+yStep].flatten(order='F')
              self.arrayInPV.put(chunk, wait=True)
              yStart += yStep
              #time.sleep(0.001)
            time.sleep(0.5)
            self.arrayCompletePV.put(1)
            rimage = self.imageArrayPV.get(count=nx*ny).reshape(nx,ny,order='F')
            result = result and self.assertTrue(np.array_equal(image, rimage))
        return result

    def testContinuousMono(self):
        # Test Mono sine waves
        result = True
        nx, ny = 500,800
        scale = 100
        loops = 50
        dt = 'float32'
        self.setupPVs(dimensions=[nx, ny], dataType=dataTypes[dt], imageMode='Continuous')
        d = dist(nx, ny)
        for i in range(1, loops+1):
            print 'testContinuousMono {0}/{1} '.format(i, loops)
            image = scale*np.sin(shift(d/i, nx/2, ny/2))
            image = image.astype(npDataTypes[dt]).transpose()
            self.acquirePV.put('Acquire')
            self.arrayInPV.put(image.flatten(order='F'), wait=True)
            rimage = self.imageArrayPV.get(count=nx*ny).reshape(nx,ny,order='F')
            result = result and self.assertTrue(np.array_equal(image, rimage))
        return result

    def testContinuousRGB1(self):
        # Test rgb1 sine waves
        result = True
        nx, ny = 600,800
        scale = 100
        loops = 20
        dt = 'int8'
        self.setupPVs(dimensions=[3, nx, ny], dataType=dataTypes[dt], imageMode='Continuous',
                      colorMode='RGB1')
        d = dist(nx, ny)
        image = np.empty([3, nx, ny])
        for i in range(1, loops+1):
            print 'testContinuousRGB1 {0}/{1} '.format(i, loops)
            red   = scale*np.sin(shift(d/i,     nx/2, ny/2))
            green = scale*np.sin(shift(d/(i*2), nx/2, ny/2))
            blue  = scale*np.sin(shift(d/(i*3), nx/2, ny/2))
            image[0,:,:] = red.transpose()
            image[1,:,:] = green.transpose()
            image[2,:,:] = blue.transpose()
            image = image.astype(npDataTypes[dt])
            self.acquirePV.put('Acquire')
            self.arrayInPV.put(image.flatten(order='F'), wait=True)
            rimage = self.imageArrayPV.get(count=3*nx*ny).reshape(3,nx,ny,order='F')
            result = result and self.assertTrue(np.array_equal(image, rimage))
        return result

    def testScanPoint(self, reverse=False):
        # Test scan record simulation in point (pixel) mode
        ndet, nx, ny = 4, 100, 200
        maxROIs = 4
        result = True
        dt = 'float64'
        if (reverse):
          yscan = [ny-1, -1, -1]
          xscan = [nx-1, -1, -1]
          nextElement = ndet * (nx*ny - 1)
        else:
          yscan = [0, ny, 1]
          xscan = [0, nx, 1]
        print 'testScanPoint reverse={0}'.format(reverse) 
        self.setupPVs(dimensions=[ndet, nx, ny], dataType=dataTypes[dt], appendMode='Enable',
                      roiSize=[1,10000,10000], imagePort='ROI1')
        self.acquirePV.put('Acquire')
        self.newArrayPV.put(1)
        counts = np.empty(ndet)
        scale = 100.
        period = 10.
        for y in range(yscan[0], yscan[1], yscan[2]):
            for x in range(xscan[0], xscan[1], xscan[2]):
                counts[0] = x + y;
                counts[1] = (x + y) * np.random.random();
                counts[2] = x*np.random.random();
                counts[3] = scale * np.sin(((x-nx/2.)**2 + (y-ny/2.)**2) / period)
                if (reverse): self.nextElementPV.put(nextElement)
                self.arrayInPV.put(counts, wait=True)
                if (reverse): nextElement -= ndet
        self.arrayCompletePV.put(1)
        for roi in range(maxROIs):
            print '  Showing ROI', roi
            self.ROIStartPVs[0].put(roi)
            self.doCallbacksPV.put(1)
            time.sleep(1)
        return result

    def testScanPointReverse(self):
        return self.testScanPoint(reverse=True)

    def testScanLine(self, detIndex=0):
        # Test scan record simulation in line mode
        ndet, nx, ny = 4, 400, 600
        maxROIs = 4
        result = True
        dt = 'float64'
        if (detIndex == 0): 
            dimensions=[ndet, nx, ny]
            roiSize=[1, nx, ny]
            stride = ndet
            yStep = ndet * nx
            detStep = 1
        elif (detIndex == 1): 
            dimensions=[nx, ndet, ny]
            roiSize=[nx, 1, ny]
            stride = 1
            yStep = ndet * nx
            detStep = nx
        elif (detIndex == 2): 
            dimensions=[nx, ny, ndet]
            roiSize=[nx, ny, 1]
            stride = 1
            yStep = nx
            detStep = nx * ny
        print 'testScanLine detector index={0}'.format(detIndex) 
        self.setupPVs(dimensions=dimensions, dataType=dataTypes[dt], appendMode='Enable', stride=stride,
                      roiSize=roiSize, imagePort='ROI1', fillValue=1)
        self.acquirePV.put('Acquire')
        self.newArrayPV.put(1)
        counts = np.empty([ndet, nx])
        scale = 100.
        period = 10.
        for y in range(ny):
            nextElement = y * yStep
            x = np.arange(nx)
            counts[0, :] = x + y;
            counts[1, :] = (x + y) * np.random.random(nx);
            counts[2, :] = x*np.random.random(nx);
            counts[3, :] = scale * np.sin(((x-nx/2.)**2 + (y-ny/2.)**2) / period)
            for det in range(ndet):
                self.nextElementPV.put(nextElement)
                self.arrayInPV.put(counts[det, :], wait=True)
                nextElement += detStep
        self.arrayCompletePV.put(1)
        for roi in range(maxROIs):
            print '  Showing ROI', roi
            self.ROIStartPVs[detIndex].put(roi)
            self.doCallbacksPV.put(1)
            time.sleep(1)
        return result

    def testScanLineIndex1(self):
        return self.testScanLine(detIndex=1)

    def testScanLineIndex2(self):
        return self.testScanLine(detIndex=2)


if __name__ == '__main__':
    unittest.main()
