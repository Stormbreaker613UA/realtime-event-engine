#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>

// Core components
#include "core/thread_pool.hpp"
#include "core/logger.hpp"

// Domain services
#include "domain/session_registry.hpp"
#include "domain/auth_service.hpp"

// Network
#include "network/server.hpp"
#include "network/session.hpp"

// Pipeline
#include "pipeline/event_router.hpp"
#include "pipeline/pipeline_context.hpp"
#include "pipeline/handlers/auth_handler.hpp"
#include "pipeline/handlers/broadcast_handler.hpp"
#include "pipeline/handlers/state_handler.hpp"

// Storage
#include "storage/async_writer.hpp"
#include "storage/istorage.hpp"

namespace storage {
    // Forward declare the FileStorage factory function
    IStoragePtr create_file_storage(const std::string& path);
}

// Global server instance for signal handler
std::atomic<bool> g_shutdown_requested{false};

void signal_handler(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        g_shutdown_requested = true;
    }
}

int main(int argc, char* argv[]) {
    try {
        // Initialize logging
        core::global_logger().set_level(core::LogLevel::Debug);
        core::global_logger().info("Starting realtime event engine server...");

        // Configuration
        const size_t THREAD_POOL_SIZE = std::thread::hardware_concurrency();
        const uint16_t SERVER_PORT = 9090;
        const std::string STORAGE_PATH = "events.log";
        const std::string SESSION_REALM = "production";

        // Setup signal handlers for graceful shutdown
        std::signal(SIGINT, signal_handler);
        std::signal(SIGTERM, signal_handler);

        // Create thread pool
        auto thread_pool = std::make_unique<core::ThreadPool>(THREAD_POOL_SIZE);
        core::global_logger().info("ThreadPool created with worker threads");

        // Create domain services
        auto session_registry = std::make_unique<domain::SessionRegistry>();
        auto auth_service = std::make_unique<domain::AuthService>();
        core::global_logger().info("Domain services initialized");

        // Create network server
        auto server = std::make_unique<network::Server>();
        core::global_logger().info("Network server created");

        // Create storage and async writer
        auto file_storage = storage::create_file_storage(STORAGE_PATH);
        auto async_writer = std::make_unique<storage::AsyncWriter>(file_storage);
        core::global_logger().info("Storage subsystem initialized");

        // Create and configure event router with handlers
        auto event_router = std::make_unique<pipeline::EventRouter>();

        // Create handlers
        auto auth_handler = std::make_shared<handlers::AuthHandler>(SESSION_REALM);
        auto broadcast_handler = std::make_shared<handlers::BroadcastHandler>();
        auto state_handler = std::make_shared<handlers::StateHandler>();

        // Register handlers for different event types
        event_router->add_handler("auth", auth_handler);
        event_router->add_handler("broadcast", broadcast_handler);
        event_router->add_handler("state", state_handler);
        core::global_logger().info("EventRouter configured with handlers");

        // Create pipeline context (wire all components together)
        pipeline::PipelineContext ctx;
        ctx.sessions = session_registry.get();
        ctx.auth = auth_service.get();
        ctx.server = server.get();

        // Set default message handler for new sessions
        server->set_default_handler([&](const network::Session::ByteVec& data) {
            core::global_logger().debug("Message received");
            // Parse and route event
            // In production, you would deserialize into core::Event here
            // For now, just log it
        });

        core::global_logger().info("=== SERVER INITIALIZATION COMPLETE ===");
        core::global_logger().info("Listening on port 9090");
        core::global_logger().info("Press Ctrl+C to shutdown gracefully");

        // Main server loop
        while (!g_shutdown_requested) {
            // Simulate server work
            // In production: accept connections, route events, etc.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // Optional: periodically log stats
            static int iterations = 0;
            if (++iterations % 100 == 0) {
                core::global_logger().debug("Server running...");
            }
        }

        // Graceful shutdown sequence
        core::global_logger().info("Shutdown signal received, initiating graceful shutdown...");

        // Stop accepting new events
        core::global_logger().info("Stopping event router...");

        // Close all sessions
        core::global_logger().info("Closing all sessions...");

        // Drain async writer queue
        core::global_logger().info("Draining storage queue...");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Destroy components in reverse order
        core::global_logger().info("Destroying components...");
        async_writer.reset();
        event_router.reset();
        server.reset();
        auth_service.reset();
        session_registry.reset();
        thread_pool.reset();

        core::global_logger().info("Shutdown complete");
        return 0;

    } catch (const std::exception& e) {
        core::global_logger().error(std::string("Fatal error: ") + e.what());
        return 1;
    } catch (...) {
        core::global_logger().error("Unknown fatal error");
        return 1;
    }
}
