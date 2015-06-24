#include <iostream>
#include <cstdlib>
#include <map>

class Tracer
{
private:
    class Entry
    {
    public:
        Entry(char const * file, int line)
            :_file(file), _line(line)
        {
            //pass
        }
        Entry()
            :_file(0), _line(0)
        {
            //pass
        }
        char const* File() const
        {
            return _file;
        }
        int Line() const
        {
            return _line;
        }
    private:
        char const* _file;
        int _line;
    };
    class Lock
    {
    public:
        Lock(Tracer& tracer)
            :_tracer(tracer)
        {
            _tracer.lock();
        }
        ~Lock()
        {
            _tracer.unlock();
        }
    private:
        Tracer& _tracer;
    };
    typedef std::map<void*, Entry>::iterator iterator;
    friend class Lock;
public:
    Tracer();
    ~Tracer();
    void Add(void* p, char const* file, int line);
    void Remove(void* p);
    void Dump();
    static bool Ready;
private:
    void lock()
    {
        ++_lockCount;
    }
    void unlock()
    {
        --_lockCount;
    }
private:
    std::map<void*, Entry> _map;
    int _lockCount;
};

void Tracer::Add(void* p, char const* file, int line)
{
    if (_lockCount > 0)
    {
        return;
    }
    Tracer::Lock lock(*this);
    _map[p] = Entry(file, line);
}

void Tracer::Remove(void* p)
{
    if (_lockCount > 0)
    {
        return;
    }
    Tracer::Lock lock(*this);
    iterator it = _map.find(p);
    if (it != _map.end())
    {
        _map.erase(it);
    }
}

void Tracer::Dump()
{
    if (_map.size() != 0)
    {
        std::cerr << "** Memory leaks(s):\n";
        for (iterator it = _map.begin(); it != _map.end(); ++it)
        {
            char const* file = it->second.File();
            int line = it->second.Line();
            unsigned int addr = reinterpret_cast<unsigned int>(it->first);
            std::cerr << "0x" << std::hex << addr << ":"
                << file << ", line" << std::dec << line
                << std::endl;
        }
        std::cerr << std::endl;
    }
}

bool Tracer::Ready = false;

Tracer::Tracer() :_lockCount(0)
{
    Ready = true;
}

Tracer::~Tracer()
{
    Ready = false;
    Dump();
}

Tracer NewTrace;

void* operator new(std::size_t size, char const* file,int line)
{

    void* p = std::malloc(size);
    if (Tracer::Ready)
    {
        NewTrace.Add(p, file, line);
    }
    return p;
}

void operator delete(void* p, char const*, int)
{
    if (Tracer::Ready)
    {
        NewTrace.Remove(p);
    }
    std::free(p);
}

void* operator new(std::size_t size)
{
    void* p = std::malloc(size);
    if (Tracer::Ready)
    {
        NewTrace.Add(p, "?", 0);
    }
    return p;
}

void operator delete(void* p)
{
    if (Tracer::Ready)
    {
        NewTrace.Remove(p);
    }
    std::free(p);
}

#define new new(__FILE__, __LINE__)

int main()
{
    int* p = new int;
}