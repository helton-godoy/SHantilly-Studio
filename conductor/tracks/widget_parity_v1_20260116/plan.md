# Implementation Plan: Remaining Widgets (V1 Parity)

Este plano detalha a expansão do Showbox Suite para atingir paridade total com os widgets da versão legada, utilizando o padrão Builder e uma abordagem declarativa.

## Phase 1: Expanded Data Contracts

Expansão das estruturas de dados em `WidgetConfigs.h` para suportar todos os novos widgets.

- [ ] Task: Define Selection and Input Configuration Structures
  - [ ] Write unit tests for CheckBox, RadioButton, SpinBox, and Slider configs
  - [ ] Implement structs: `CheckBoxConfig`, `RadioButtonConfig`, `SpinBoxConfig`, `SliderConfig`
  - [ ] Verify default values and validation logic
- [ ] Task: Define Utility and Text Configuration Structures
  - [ ] Write unit tests for Calendar, TextEdit, and Separator configs
  - [ ] Implement structs: `CalendarConfig`, `TextEditConfig`, `SeparatorConfig`
- [ ] Task: Define Container Configuration Structures (Hierarchical)
  - [ ] Write unit tests for GroupBox, Frame, and TabWidget configs (including nesting)
  - [ ] Implement structs: `GroupBoxConfig`, `FrameConfig`, `PageConfig`, `TabWidgetConfig`
- [ ] Task: Conductor - User Manual Verification 'Phase 1: Expanded Contracts' (Protocol in workflow.md)

## Phase 2: Selection and Utility Widget Implementation

Implementação dos controles de seleção e utilitários visuais simples.

- [ ] Task: Implement `ShowboxBuilder` for Selection Widgets
  - [ ] Write failing tests for CheckBox and RadioButton instantiation
  - [ ] Implement `buildCheckBox` and `buildRadioButton`
  - [ ] Verify signal/slot compatibility for passive view
- [ ] Task: Implement `ShowboxBuilder` for Utility Widgets
  - [ ] Write failing tests for Separator and Calendar
  - [ ] Implement `buildSeparator` and `buildCalendar`
- [ ] Task: Conductor - User Manual Verification 'Phase 2: Selection and Utilities' (Protocol in workflow.md)

## Phase 3: Numerical and Text Input Implementation

Implementação de controles de entrada de dados mais complexos.

- [ ] Task: Implement `ShowboxBuilder` for Numerical Inputs
  - [ ] Write failing tests for SpinBox and Slider (Min/Max/Step validation)
  - [ ] Implement `buildSpinBox` and `buildSlider`
- [ ] Task: Implement `ShowboxBuilder` for Multiline Text
  - [ ] Write failing tests for TextEdit (Rich Text support)
  - [ ] Implement `buildTextEdit`
- [ ] Task: Conductor - User Manual Verification 'Phase 3: Inputs' (Protocol in workflow.md)

## Phase 4: Container and Hierarchical Implementation

Implementação dos contêineres que permitem organizar outros widgets.

- [ ] Task: Implement `ShowboxBuilder` for Basic Containers
  - [ ] Write failing tests for GroupBox and Frame (Layout nesting)
  - [ ] Implement `buildGroupBox` and `buildFrame`
- [ ] Task: Implement `ShowboxBuilder` for Tabbed Interfaces (Block Construction)
  - [ ] Write failing tests for TabWidget with multiple Pages
  - [ ] Implement `buildTabWidget` using the "Block Construction" approach
  - [ ] Verify child widget reparenting and layout management
- [ ] Task: Conductor - User Manual Verification 'Phase 4: Containers' (Protocol in workflow.md)

## Phase 5: Final Parity Validation

Validação final de que todos os recursos do V1 estão acessíveis via nova arquitetura.

- [ ] Task: Comprehensive Integration Demo
  - [ ] Create a complex UI PoC using every widget implemented in this track
  - [ ] Verify visual alignment and behavior in the Dev Container
- [ ] Task: Conductor - User Manual Verification 'Phase 5: Parity Validation' (Protocol in workflow.md)
