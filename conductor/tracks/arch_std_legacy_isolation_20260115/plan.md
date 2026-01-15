# Implementation Plan: Architecture Standardization & Legacy Isolation

Este plano descreve a transição para a arquitetura definitiva do Showbox Suite, focada em modularidade (CMake), desacoplamento (Builder Pattern) e paridade entre CLI e Studio.

## Task Snapshot

- **Primary goal:** Isolar o código legado (V1/V2) e implementar um "Golden Sample" (PushButton) na nova arquitetura.
- **Success signal:** Parser novo processando `add pushbutton` via `ShowboxBuilder` com paridade visual ao legado.
- **Tech Stack:** C++17, Qt6, CMake, Docker.

---

## Phase 1: Saneamento e Isolamento do Legado

**Objetivo:** Limpar a raiz do projeto e organizar o código antigo em pastas de "Quarentena" sem quebrar a funcionalidade.

- [x] Task: Criar estrutura de diretórios para legado (`src/legacy/v1_monolith` e `src/legacy/v2_incomplete`)
- [x] Task: Mover arquivos do Parser V1 para `src/legacy/v1_monolith`
- [x] Task: Mover arquivos do Parser V2 para `src/legacy/v2_incomplete`
- [x] Task: Atualizar `showbox.pro` (Build Legado) para refletir os novos caminhos
- [x] Task: Validar que o binário legado (`showbox`) continua compilando via `./src/start-dev.sh`
- [~] Task: Conductor - User Manual Verification 'Phase 1: Saneamento e Isolamento do Legado' (Protocol in workflow.md)

## Phase 2: Refatoração do Widget PushButton (Passive View)

**Objetivo:** Garantir que o PushButton na `libs/showbox-ui` seja uma "View Passiva" pura.

- [ ] Task: Revisar `libs/showbox-ui/src/push_button_widget.cpp` (ou similar) para remover qualquer lógica de shell script
- [ ] Task: Escrever testes unitários para `PushButton` validando propriedades (texto, ícone, estado) em `tests/auto/tst_pushbutton`
- [ ] Task: Implementar/Ajustar `PushButton` para passar nos testes unitários
- [ ] Task: Conductor - User Manual Verification 'Phase 2: Refatoração do Widget PushButton' (Protocol in workflow.md)

## Phase 3: Implementação do Builder Pattern

**Objetivo:** Criar a camada de abstração que o Studio e o CLI usarão para construir interfaces.

- [ ] Task: Definir interface `IShowboxBuilder` em `libs/showbox-ui/include/`
- [ ] Task: Criar classe `CLIBuilder` em `src/core/builder/` implementando a interface
- [ ] Task: Escrever testes unitários para o `CLIBuilder` (Mocking de widgets)
- [ ] Task: Implementar criação de `PushButton` no `CLIBuilder` consumindo a `WidgetFactory`
- [ ] Task: Conductor - User Manual Verification 'Phase 3: Implementação do Builder Pattern' (Protocol in workflow.md)

## Phase 4: Novo Parser e Comando de Referência

**Objetivo:** Implementar o novo loop de parsing minimalista e o comando `add` para o PushButton.

- [ ] Task: Criar estrutura do novo loop de parsing em `src/core/parser/ParserMain.cpp`
- [ ] Task: Escrever testes unitários para o parsing do comando `add pushbutton`
- [ ] Task: Implementar lógica do comando `add` vinculada ao `ShowboxBuilder`
- [ ] Task: Integrar o novo parser no binário principal `showbox` (nova versão)
- [ ] Task: Conductor - User Manual Verification 'Phase 4: Novo Parser e Comando de Referência' (Protocol in workflow.md)

## Phase 5: Validação e Documentação de Contexto

**Objetivo:** Provar a paridade e blindar o contexto para as próximas sessões.

- [ ] Task: Criar script de teste comparativo `tests/manual/compare_v1_vs_target.sh`
- [ ] Task: Atualizar `.gemini/GEMINI.md` com a regra de "Golden Sample" apontando para a nova arquitetura
- [ ] Task: Remover referências obsoletas de documentação que causem confusão entre V1 e V2
- [ ] Task: Conductor - User Manual Verification 'Phase 5: Validação e Documentação de Contexto' (Protocol in workflow.md)
