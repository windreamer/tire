#ifndef __PTHREADXX__
#define __PTHREADXX__

#include <stdexcept>
#include <pthread.h>
#include <errno.h>
#include <memory>

namespace pthreadxx
{

struct exception : public std::runtime_error
{
    explicit exception (const std::string& what):
        std::runtime_error(what)
    {
    }

    virtual ~exception() throw()
    {
    }

}; // struct exception

struct invalid_state : public exception
{
    explicit invalid_state (const std::string& what):
        exception(what)
    {   
    }

    virtual ~invalid_state() throw()
    {
    }

}; // struct invalid_state

struct thread_attribute
{
    thread_attribute()
    {
        int ret = pthread_attr_init(&attr);
        if (ret)
        {
            throw std::bad_alloc();
        }
    }

    virtual ~thread_attribute()
    {
        pthread_attr_destroy(&attr);
    }

    int detach_state () const
    {
        int result = 0;
        int ret = pthread_attr_getdetachstate(&attr, &result);
        if (ret)
        {
            throw invalid_state("pthread_attr_getdetachstate failed");
        }
        return result;
    }

    void detach_state (int state)
    {
        int ret = pthread_attr_setdetachstate(&attr, state);
        if (ret)
        {
            throw invalid_state("pthread_attr_setdetachstate failed");
        }
    }

    size_t guard_size () const
    {
        size_t result = 0;
        int ret = pthread_attr_getguardsize(&attr, &result);
        if (ret)
        {
            throw invalid_state("pthread_attr_getguardsize failed");
        }
        return result;
    }

    void guard_size (size_t size)
    {
        int ret = pthread_attr_setguardsize(&attr, size);
        if (ret)
        {
            throw invalid_state("pthread_attr_setguardsize failed");
        }
    }

    size_t stack_size () const
    {
        size_t result = 0;
        int ret = pthread_attr_getstacksize(&attr, &result);
        if (ret)
        {
            throw invalid_state("pthread_attr_getstacksize failed");
        }
        return result;
    }

    void stack_size (size_t size)
    {
        int ret = pthread_attr_setstacksize(&attr, size);
        if (ret==EINVAL)
        {
            throw std::invalid_argument(
                    "pthread_attr_setstacksize size too small");
        }
        else if(ret)
        {
            throw invalid_state("pthread_attr_setstacksize failed");
        }
    }

    pthread_attr_t id () const
    {
        return attr;
    }

    protected:
        pthread_attr_t attr;
    private:
        thread_attribute(const thread_attribute&);
        thread_attribute& operator = (const thread_attribute&);
        
}; // struct thread_attribute

struct thread
{
    template<typename Functor>
    static thread create (const Functor& f, const thread_attribute& attribute)
    {
        return create(f, &(attribute.id()));
    }

    template<typename Functor>
    static thread create (const Functor& f)
    {
        return create(f, NULL);
    }

    static thread current()
    {
        return thread(pthread_self());
    }

    explicit thread ():
        is_valid(false)
    {
    }

    thread (const thread& rhs):
        is_valid(rhs.is_valid), thread_handle(rhs.thread_handle)
    {
    }

    virtual ~thread()
    {
    }

    bool valid () const
    {
        return is_valid;
    }
    
    pthread_t id () const
    {
        return thread_handle;
    }

    thread& operator = (const thread& rhs)
    {
        if (&rhs == this)
        {
            return *this;
        }
        is_valid = rhs.is_valid;
        thread_handle = rhs.thread_handle;
        return *this;
    }

    bool operator == (const thread& rhs) const
    {
        if (!is_valid)
        {
            return !rhs.is_valid;
        }
        return pthread_equal(thread_handle, rhs.thread_handle) != 0;
    }

    bool operator != (const thread& rhs) const
    {
        return !(operator==(rhs));
    }
    
    void* join () const
    {
        if (!is_valid)
        {
            throw invalid_state("pthread_join thread invalid");
        }

        void* result = NULL;
        int ret = pthread_join(thread_handle, &result);
        if (ret == EINVAL || ret == ESRCH)
        {
            throw std::invalid_argument("pthread_join no such thread");
        }
        else if (ret)
        {
            throw invalid_state("pthread_join failed");
        }

        return result;
    }
    
    void detach () const
    {
        if (!is_valid)
        {
            throw invalid_state("pthread_detach thread invalid");
        }
        int ret = pthread_detach(thread_handle);
        if (ret)
        {
            throw invalid_state("pthread_detach failed");
        }
    }
    
    protected:
        pthread_t thread_handle;
        bool is_valid;

        explicit thread (const pthread_t handle):
            is_valid(true), thread_handle(handle)
        {
        }
        
        template<typename Functor>
        static thread create(const Functor& f, const pthread_attr_t* attr)
        {
            std::auto_ptr<Functor> functor(new Functor(f));
            pthread_t handle;
            int ret = pthread_create(&handle, attr, thread_proc<Functor>, 
                    static_cast<void*>(functor.get()));
            if (ret == EAGAIN)
            {
                throw std::bad_alloc();
            }
            else if (ret)
            {
                throw invalid_state("pthread_create failed");
            }
            functor.release();
            return thread(handle);
        }
        
        template<typename Functor>
        static void* thread_proc (void* f)
        {
            std::auto_ptr<Functor> functor(static_cast<Functor*>(f));
            return (*functor)();
        }

}; // struct thread

} // namespace pthreadxx

#endif //__PTHREADXX__

// vim: set filetype=cpp
