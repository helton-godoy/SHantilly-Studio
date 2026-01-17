# Implementation Plan: V1 Parser Modernization

Este plano detalha a implementação do novo sistema de parsing para conectar a CLI legado à nova arquitetura do Builder.

## Phase 1: Core Infrastructure

Estabelecimento das fundações para processamento de texto e comandos.

- [x] Task: Implement `Tokenizer` Class [60f5559]
  - [x] Write tests for splitting strings with quotes, spaces, and escapes
  - [x] Implement `Tokenizer` class (replaces naive split)
- [x] Task: Implement `CommandParser` Skeleton [dc666cd]
  - [x] Define `ICommand` interface or Command structure
  - [x] Implement dispatch logic (Map<String, Handler>)
  - [x] Connect `ParserMain` to use `CommandParser` [checkpoint: e7f1b89]

## Phase 2: Widget Creation (The 'Add' Command) - Part 1

Suporte para widgets básicos e propriedades comuns.

- [ ] Task: Implement Generic Property Parser
  - [ ] Logic to parse common flags: `text`, `title`, `name`, `width`, `height`, `checked`, `enabled`, `visible`
- [ ] Task: Implement Parsers for Basic Widgets
  - [ ] `add label`, `add button` (pushbutton)
  - [ ] `add window` (main window config)
- [ ] Task: Implement Parsers for Selection Widgets
  - [ ] `add checkbox`, `add radiobutton`
  - [ ] `add combobox`, `add listbox` (parsing items lists)

## Phase 3: Widget Creation (The 'Add' Command) - Part 2

Suporte para widgets complexos e contêineres.

- [ ] Task: Implement Parsers for Numerical/Text Inputs
  - [ ] `add spinbox`, `add slider` (min/max/step)
  - [ ] `add lineedit` (textbox), `add textedit` (textview)
- [ ] Task: Implement Parsers for Containers
  - [ ] `add groupbox`, `add frame`
  - [ ] `add tabwidget`, `add page` (handling context/hierarchy)

## Phase 4: Runtime Interaction

Suporte para modificar o estado da UI após a criação.

- [ ] Task: Implement `Set` and `Unset` Commands
  - [ ] Map property names to `IShowboxBuilder` or direct widget manipulation (TBD: Bridge pattern might be needed here or access via `StudioController`)
- [ ] Task: Implement `Query` Command
  - [ ] Logic to retrieve values from widgets by name

## Phase 5: Integration & Validation

- [ ] Task: End-to-End Integration
  - [ ] Replace `poc_modern_cli` input loop with full `ParserMain`
  - [ ] Verify against legacy test scripts (e.g., `examples/showbox_demo.sh`)
- [ ] Task: Conductor - User Manual Verification 'Phase 5: Parser Validation'
