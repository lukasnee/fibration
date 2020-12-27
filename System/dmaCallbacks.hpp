#pragma once 

class DmaCallbacks 
{
public:
    DmaCallbacks(){};
    virtual void dmaTxCpltCallback() {};
    virtual void dmaTxHalfCpltCallback() {};
    virtual void dmaRxCpltCallback() {};
    virtual void dmaRxHalfCpltCallback() {};
    virtual void dmaErrorCallback() {}; 
    virtual void dmaAbortCpltCallback() {};
    virtual void dmaAbortTransmitCpltCallback() {};
    virtual void dmaAbortReceiveCpltCallback() {};
};
