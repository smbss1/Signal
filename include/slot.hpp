//
// Created by samuel on 01/11/2021.
//

#ifndef SIGNAL_SLOT_SLOT_HPP
#define SIGNAL_SLOT_SLOT_HPP

#include <memory>
#include <utility>
#include <vector>
#include <functional>
#include <utility>

namespace fox
{
    class slot_state;

    /**
     * @brief The class who have the responsability of removing the slot to a container
     */
    struct cleaner
    {
        virtual ~cleaner() = default;
        virtual void clean(slot_state *) = 0;
    };

    /**
     * It's responsible of the state of the slot
     */
    class slot_state
    {
    public:
        slot_state();

        virtual ~slot_state() = default;

        virtual bool is_connected() const noexcept;
        bool disconnect() noexcept;

        std::size_t get_index() const;

        void set_index(std::size_t idx);

    protected:
        virtual void do_disconnect() = 0;

    private:
        std::size_t m_lIndex;
        bool m_bConnected;
    };

    /**
     * It's the class base of the slot, responsible of call clean from the cleaner class
     * @tparam Types The paramaters type of the function
     */
    template<typename ...Types>
    class slot_base : public slot_state
    {
    public:
        slot_base(cleaner &c) : slot_state(), m_oCleaner(c)
        {}

        virtual ~slot_base() = default;

        virtual void call(Types...) = 0;

    protected:
        void do_disconnect() final override
        {
            m_oCleaner.clean(this);
        }

    private:
        cleaner &m_oCleaner;
    };

    /**
     * @brief It's the slot class responsible of all the function call
     * @tparam Func The signature of the function
     * @tparam Types The paramaters type of the function
     */
    template<typename Func, typename ...Types>
    class slot : public slot_base<Types...>
    {
    public:
        template<typename F>
        constexpr slot(cleaner &c, F && f)
            : slot_base<Types...>(c), m_oFunc(std::forward<F>(f))
        {}

        virtual ~slot() = default;

        void call(Types... args)
        {
            m_oFunc(args...);
        }

    private:
        std::decay_t<Func> m_oFunc;
    };

    /**
     * @brief It's resposible of the connection between the slot and the signal who it linked
     */
    class connection
    {
    public:
        explicit connection(std::weak_ptr<slot_state> slot);
        virtual ~connection() = default;

        bool is_connected() const noexcept;
        bool disconnect() noexcept;

    protected:
        std::weak_ptr<slot_state> m_oSlot;
    };
}

#endif //SIGNAL_SLOT_SIGNAL_HPP
