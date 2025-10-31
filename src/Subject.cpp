#include "Subject.h"
#include "Observer.h"

void Subject::attach(Observer* observer)
{
    if (observer) {
        observerCollection.push_back(observer);
    }
}

void Subject::detach(Observer* observer)
{
    observerCollection.erase(
        std::remove(observerCollection.begin(), observerCollection.end(), observer),
        observerCollection.end()
    );
}

void Subject::notifyAll()
{
    // Posielame LEN subject, Observer si vytiahne, čo potrebuje!
    for (Observer* observer : observerCollection)
    {
        if (observer) {
            observer->notify(this);  // LEN subject pointer, niečo sa zmenilo
        }
    }
}