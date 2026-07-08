// server.cpp

#include "server.hpp"

namespace network
{

    Server::SessionPtr Server::create_session(const std::string& id)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (auto it = sessions_.find(id); it != sessions_.end())
            return it->second;

        auto session = std::make_shared<Session>(id);

        if (default_handler_)
            session->set_message_handler(default_handler_);

        sessions_.emplace(id, session);

        return session;
    }

    bool Server::remove_session(const std::string& id)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return sessions_.erase(id) != 0;
    }

    bool Server::deliver_bytes(const std::string& id,
        const uint8_t* data,
        std::size_t length)
    {
        SessionPtr session;

        {
            std::lock_guard<std::mutex> lock(mutex_);

            auto it = sessions_.find(id);
            if (it == sessions_.end())
                return false;

            session = it->second;
        }

        // Process outside the mutex.
        session->on_receive(data, length);

        return true;
    }

    std::optional<Server::ByteVec>
        Server::prepare_send(const std::string& id,
            const ByteVec& payload)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (sessions_.find(id) == sessions_.end())
            return std::nullopt;

        return Session::prepare_send(payload);
    }

    std::vector<std::pair<std::string, Server::ByteVec>>
        Server::prepare_broadcast(const ByteVec& payload)
    {
        std::vector<std::pair<std::string, ByteVec>> packets;

        ByteVec framed = Session::prepare_send(payload);

        std::lock_guard<std::mutex> lock(mutex_);

        packets.reserve(sessions_.size());

        for (const auto& [id, session] : sessions_)
        {
            packets.emplace_back(id, framed);
        }

        return packets;
    }

    void Server::set_default_handler(Session::MessageHandler handler)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        default_handler_ = std::move(handler);

        for (auto& [id, session] : sessions_)
        {
            if (session)
                session->set_message_handler(default_handler_);
        }
    }

    std::size_t Server::session_count() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return sessions_.size();
    }

} // namespace network
