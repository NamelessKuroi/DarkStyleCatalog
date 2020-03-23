#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T> struct Singleton
{
    static T &instance()
    {
        static T m_instance;
        return m_instance;
    }
protected:
    Singleton() { }
};

#endif // SINGLETON_H
