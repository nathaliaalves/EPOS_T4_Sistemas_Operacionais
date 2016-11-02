// EPOS Semaphore Abstraction Implementation

#include <semaphore.h>

__BEGIN_SYS

Semaphore::Semaphore(int v): _value(v)
{
    db<Synchronizer>(TRC) << "Semaphore(value=" << _value << ") => " << this << endl;
}


Semaphore::~Semaphore()
{
    db<Synchronizer>(TRC) << "~Semaphore(this=" << this << ")" << endl;
}


void Semaphore::p()
{
    db<Synchronizer>(TRC) << "Semaphore::p(this=" << this << ",value=" << _value << ")" << endl;

    begin_atomic();
    if(fdec(_value) < 1)
   {
        Thread::Priority p = Thread::self()->priority();
        if(this->acquired_resource->priority() < Thread::self()->priority()) {
            this->acquired_resource->priority(p);
        }
        sleep(); // implicit end_atomic()
    }
    else {
        this->acquired_resource = Thread::self();
        end_atomic();
    }
}


void Semaphore::v()
{
    db<Synchronizer>(TRC) << "Semaphore::v(this=" << this << ",value=" << _value << ")" << endl;

    begin_atomic();

    this->acquired_resource->reset_priority();

    if(finc(_value) < 0)
        wakeup();  // implicit end_atomic()
    else
        end_atomic();
}

__END_SYS
