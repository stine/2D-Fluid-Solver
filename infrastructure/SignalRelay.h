#ifndef __SIGNALRELAY_H__
#define __SIGNALRELAY_H__

#include <QtGui>
#include <QObject>

// This class is used as a relay for Qt signals to eventually reach Qt slots
// throughout the simulation.  Rather than performing the QObject::connect()
// inside a UI class (which would require knowledge of the 'slot' objects),
// or performing the connect() inside the simulation (which would require
// knowledge of the UI classes), this passthrough should be used.

// Essentially, a QObject::connect() should be performed between the origin
// signal inside a UI class and a signal listed in this class.  Then, any
// potential listeners for the signal can simply connect to the signalRelay
// class without knowledge of the various UI classes.  This allows for loose
// coupling of components in the system.

// This class is implemented as a singleton.

class SignalRelay : public QObject
{
  Q_OBJECT

  public:
    static SignalRelay * getInstance();
    
  signals:
    // Extend this with additional signals here!
    void resetSimulation();

  private:
    SignalRelay() {}
    static SignalRelay *_instance;
};

#endif // __SIGNALRELAY_H__
