# Library Management System (C++ OOP)

A robust, object-oriented Library Management System developed in **C++20** featuring custom data abstractions, design patterns, SQLite persistence, and interactive CLI management.

---

## Features & User Flow
- **Interactive Menu (`Meniu`):** User-friendly CLI interface for navigation.
- **Catalog Management:** Add, update, remove, and search for books and authors.
- **Borrowing Operations:** Issue books to readers with automated status updates.
- **Member Administration:** Manage reader profiles and staff credentials.
- **Data Persistence:** All transactions and state changes are saved to `librarie.db`.

---

## Architecture & Core OOP Features

This project demonstrates advanced **Object-Oriented Programming (OOP)** principles and C++ software engineering design practices:

### 1. Object Hierarchy & Polymorphism
- **`IObject` Interface:** Base interface ensuring consistent string serialization and display capabilities.
- **`Persoana` Base Class:** Abstract base class for human entities in the system.
  - **`Cititor`** (Reader): Handles reader profiles, borrowed books, and library history.
  - **`Angajat`** (Employee): Represents staff members with administrative privileges and duties.
  - **`AngajatCititor`:** Demonstrates multiple/diamond inheritance mechanics by bridging reader and staff behaviors.
- **`Carte` & `Autor`:** Domain models for managing catalog items and author details.

### 2. Design Patterns & Advanced Constructs
- **Abstract Factory (`AbstractFactory.h`):** Provides an interface for creating families of related domain objects dynamically.
- **Generic Collections (`Colectie.h`):** Custom template wrapper managing dynamic collections and custom operations.
- **Configuration Singleton (`ConfigurareLibrarie.h`):** Global access point for system settings and runtime configuration.
- **Custom Exceptions (`Exceptii.h`):** Hierarchy of exception classes handling domain-specific runtime errors (e.g., book unavailable, invalid entity state).

### 3. Persistence & Database Integration
- **SQLite Engine (`Database.h` / `librarie.db`):** Native C++ integration with SQLite for database CRUD operations.
- **Seed Data (`seed.sql`):** Pre-configured SQL scripts for initializing tables and populating demo records.
- **Logging System (`logs.txt`):** Tracks system operations and errors.
