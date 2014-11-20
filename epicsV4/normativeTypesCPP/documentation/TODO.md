TODO
===========

NTScalarArray
------------

NTScalarArrayBuilder::arrayValue

Should this be:

NTScalarArrayBuilder::value

This makes NTScalarArray consistent with other NTTypes.


NTNameValue
------------

This does not have a value field like all the other NTTypes.
Perhaps instead of

    NTNameValue
        string[] name
        string[] value

It should be
        
    NTNameValue
        value
            string[] name
            string[] value

NTTable
----------

Should lables be label?
Compare with name,value from NTNameValue.
