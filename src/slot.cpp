//
// Created by samuel on 02/11/2021.
//

#include "slot.hpp"

namespace fox
{
    slot_state::slot_state() : m_bConnected(true)
    {}

    bool slot_state::is_connected() const noexcept
    {
        return m_bConnected;
    }

    bool slot_state::disconnect() noexcept
    {
        bool ret = m_bConnected;
        if (ret) {
            do_disconnect();
            m_bConnected = false;
        }
        return ret;
    }

    std::size_t slot_state::get_index() const
    {
        return m_lIndex;
    }

    void slot_state::set_index(std::size_t idx)
    {
        m_lIndex = idx;
    }

    connection::connection(std::weak_ptr<slot_state> slot) : m_oSlot(std::move(slot))
    {}

    bool connection::is_connected() const noexcept
    {
        const auto d = m_oSlot.lock();
        return d && d->is_connected();
    }

    bool connection::disconnect() noexcept
    {
        auto d = m_oSlot.lock();
        return d && d->disconnect();
    }
}
