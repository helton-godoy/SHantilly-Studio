# Implementation Plan: Showbox Architecture Modernization

Este plano detalha a refatoração profunda do núcleo do Showbox, implementando o padrão Builder e estruturas de dados modernas para suportar o Showbox Studio e um novo protocolo de comunicação.

## Phase 1: Foundation - Interfaces and Data Contracts

Esta fase estabelece as bases da nova arquitetura, definindo como os widgets são descritos e a interface de construção.

- [x] Task: Define Widget Configuration Structures (Data Contracts) fc35ca1
  - [ ] Write unit tests for config structure defaults and validation
  - [ ] Implement C++ structs for Widget Properties (Base, Button, Label, Window)
  - [ ] Verify coverage and adherence to C++17 standards
- [ ] Task: Define `IShowboxBuilder` Interface
  - [ ] Create the header `libs/showbox-ui/include/IShowboxBuilder.h` with the comprehensive interface
  - [ ] Ensure all proposed widgets are represented in the interface methods
  - [ ] Document the interface following project guidelines
- [ ] Task: Conductor - User Manual Verification 'Phase 1: Foundation' (Protocol in workflow.md)

## Phase 2: Core Builder Implementation - Basic Widgets

Implementação da lógica real de criação dos widgets fundamentais.

- [ ] Task: Implement `ShowboxBuilder` for Window and Basic Layouts
  - [ ] Write failing tests for Window/Layout creation via Builder
  - [ ] Implement `createWindow` and basic layout methods in `ShowboxBuilder`
  - [ ] Verify tests and measure performance
- [ ] Task: Implement `ShowboxBuilder` for Basic Controls (Button, Label, LineEdit)
  - [ ] Write failing tests for basic control instantiation
  - [ ] Implement `createButton`, `createLabel`, and `createLineEdit`
  - [ ] Verify that widgets are "Passive" (no internal shell logic)
- [ ] Task: Conductor - User Manual Verification 'Phase 2: Core Builder Implementation' (Protocol in workflow.md)

## Phase 3: Complex Widgets and Data Displays

Migração de componentes mais avançados para o novo padrão.

- [ ] Task: Implement `ShowboxBuilder` for Item-based Widgets (List, Table, ComboBox)
  - [ ] Write failing tests for data-heavy widgets
  - [ ] Implement methods for List, Table and Combo creation and data population
  - [ ] Verify data binding and display performance
- [ ] Task: Implement `ShowboxBuilder` for Advanced Visuals (Charts, ProgressBars)
  - [ ] Write failing tests for `CustomChartWidget` and ProgressBar integration
  - [ ] Implement `createChart` and `createProgressBar` using existing Core visuals
  - [ ] Verify visual fidelity with legacy components
- [ ] Task: Conductor - User Manual Verification 'Phase 3: Complex Widgets' (Protocol in workflow.md)

## Phase 4: Integration and Validation

Garantir que o novo sistema está pronto para ser consumido e substituir o legado.

- [ ] Task: Create "Modern CLI" Proof of Concept
  - [ ] Implement a minimal CLI entry point that uses the new Builder
  - [ ] Demonstrate a complex UI being built without the legacy parser
  - [ ] Validate stdout interaction/feedback loop for Shell Scripts
- [ ] Task: Legacy Bridge/Cleanup Assessment
  - [ ] Evaluate if a compatibility layer is needed or if we proceed with full cut-off
  - [ ] Document the new "Showbox Protocol" for Shell Users
- [ ] Task: Conductor - User Manual Verification 'Phase 4: Integration' (Protocol in workflow.md)
