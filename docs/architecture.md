# Architecture Overview

This document provides a short overview of the project's high-level architecture.

- core: utilities and primitives used across the system (events, thread pool, queues, logging).
- network: framing and session abstractions; server-level session management.
- domain: application domain objects and services (session registry, entity state, auth service).
- pipeline: event routing and pipeline context for handlers.
- handlers: small pluggable handlers that operate on events (auth, state, broadcast).
- Storage: storage abstractions and implementations (IStorage, file storage, async writer).
- tests: small unit tests for core components.
- benchmarks: micro-benchmarks for throughput measurements.

The system is designed to be modular: the event pipeline accepts events and routes them to
handlers via the EventRouter. Handlers may interact with domain services and the network server
through the PipelineContext.
