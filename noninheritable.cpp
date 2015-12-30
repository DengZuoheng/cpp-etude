#ifndef ETUDE_NONINHERITABLE_H
#define ETUDE_NONINHERITABLE_H

namespace etude{
    namespace _noninheritable{
        class noninheritable_base{
        protected:
            noninheritable_base(){}
            ~noninheritable_base(){}
        };
        class noninheritable :public virtual noninheritable_base{
        protected:
            noninheritable(){}
            ~noninheritable(){}
        };
    };
    typedef _noninheritable::noninheritable noninheritable;
#define ETUDE_FINAL private etude::noninheritable
};

#endif