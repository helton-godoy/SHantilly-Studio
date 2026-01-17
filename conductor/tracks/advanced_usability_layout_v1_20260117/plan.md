# Implementation Plan: Advanced Usability & Layouts

## Phase 1: Undo/Redo Infrastructure [checkpoint: 90f2b76]

Implementar a rede de segurança para edição.

- [~] Task: Setup `QUndoStack` in `StudioController`
  - [ ] Initialize stack.
  - [ ] Expose Undo/Redo actions to MainWindow.
- [x] Task: Implement `Add/Delete` Commands [1d0a03c]
  - [ ] Refactor `MainWindow::onDeleteClicked` to push a `DeleteWidgetCommand`.
  - [ ] Refactor `Canvas::dropEvent` to push an `AddWidgetCommand`.
- [x] Task: Implement `PropertyChange` Command [474293f]
  - [ ] Intercept changes in `PropertyEditor` and push commands instead of direct set.

## Phase 2: Hierarchy Reordering (Tree View)

Permitir organizar a estrutura via Drag & Drop na árvore.

- [x] Task: Enable Drag & Drop in `ObjectInspector` [22051a4]
  - [ ] Set `dragDropMode` to `InternalMove`.
  - [ ] Override `dropEvent` to handle reparenting logic.
- [~] Task: Synchronize with Canvas
  - [ ] Implement logic to move the actual QWidget when the TreeItem is moved.
  - [ ] Ensure `layout()->insertWidget(index, w)` is used.

## Phase 3: Contextual Layout Grouping

Facilitar a criação de layouts complexos.

- [ ] Task: Implement Context Menu in Canvas/Tree
  - [ ] Actions: "Group in Frame (HBox)", "Group in Frame (VBox)".
- [ ] Task: Implement Grouping Logic
  - [ ] Create container.
  - [ ] Reparent selected widgets to new container.
  - [ ] Handle Undo/Redo for grouping (complex composite command).

## Phase 4: Layout Properties & Refinement

- [ ] Task: Expose Layout Configuration
  - [ ] Show `layout` property in `PropertyEditor` for containers.
  - [ ] Allow switching between HBox/VBox dynamically.
