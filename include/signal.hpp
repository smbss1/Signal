//
// Created by samuel on 01/11/2021.
//

#ifndef SIGNAL_SLOT_SIGNAL_HPP
#define SIGNAL_SLOT_SIGNAL_HPP

#include <memory>
#include <utility>
#include <vector>
#include <functional>
#include <utility>

#include "slot.hpp"

namespace fox
{
    /**
     * @brief Resposible of emit the signal
     * @tparam Types The paramaters type of the function
     */
    template<typename ...Types>
    class signal : public cleaner
    {
        using slot_type = slot_base<Types...>;
        using slot_ptr_type = std::shared_ptr<slot_type>;
        using list_type = std::vector<slot_ptr_type>;

    public:
        ~signal() override = default;

        /**
         * @brief Connect a callable function to this Signal
         * @tparam Callable The signature of the function
         * @param callable the function
         * @return a Connection class
         */
        template<typename Callable>
        connection connect(Callable &&callable)
        {
            using slot_t = slot<Callable, Types...>;

            auto pSlot = make_slot<slot_t>(std::forward<Callable>(callable));
            m_vSlots.push_back(pSlot);
            return connection(pSlot);
        }

        // Convenience method to connect a member function of an
        // object to this Signal.
        /**
         * @brief Convenience method to connect a member function of an
         * object to this Signal.
         * @tparam T The Instance Type
         * @tparam Callable The signature of the function
         * @param inst the instance
         * @param func the member function of the object
         * @return a Connection class
         */
        template<typename T, typename Callable>
        connection connect(T *inst, Callable &&func)
        {
            return connect([=](Types... args)
                           {
                               (inst->*func)(args...);
                           });
        }

        /**
         * @brief Remove all the slot to this Signal
         * @return the number of slot who has been remove
         */
        std::size_t disconnect_all()
        {
            return disconnect_if([&](const auto& slot){
                return true;
            });
        }

        /**
         * @brief Get the number of slots contains in this Signal
         * @return the number of slots contains in this Signal
         */
        typename list_type::size_type size() const
        {
            return m_vSlots.size();
        }

        /**
         * @brief Send the signal to the slots with the correct arguments
         * @param args the arguments pass for the slots
         */
        void emit(Types... args)
        {
            for (auto &slot: m_vSlots)
                slot->call(args...);
        }

        void clean(slot_state *slot) override
        {
            std::size_t index = slot->get_index();

            if (index < m_vSlots.size())
            {
                if (m_vSlots[index] && m_vSlots[index].get() == slot)
                {
                    std::swap(m_vSlots[index], m_vSlots.back());
                    m_vSlots[index]->set_index(index);
                    m_vSlots.pop_back();
                }
            }
        }

    private:
        // create a new slot
        template<typename Slot, typename... A>
        inline auto make_slot(A &&...a)
        {
            return slot_ptr_type(static_cast<slot_type *>(new Slot(*this, std::forward<A>(a)...)));
        }

        /**
         * @brief Disconnect a slot if a condition occurs
         * @tparam Cond Func signature as bool(slot_ptr_type)
         * @param cond the function who test the condition
         * @return the number of removed slots using the condition function
         */
        template <typename Cond>
        size_t disconnect_if(Cond && cond)
        {
            size_t count = 0;
            for (int i = 0; i < m_vSlots.size();)
            {
                if (cond(m_vSlots[i])) {
                    std::swap(m_vSlots[i], m_vSlots.back());
                    m_vSlots[i]->set_index(i);
                    m_vSlots.pop_back();
                    ++count;
                } else {
                    ++i;
                }
            }
            return count;
        }


    private:
        list_type m_vSlots;
    };
}

#endif //SIGNAL_SLOT_SIGNAL_HPP
