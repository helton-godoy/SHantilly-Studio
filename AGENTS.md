# AGENTS.md

## Project Overview

SHantilly-Studio is a Visual IDE for SHantilly, built with C++/Qt6. It allows users to design SHantilly GUIs visually and generate the corresponding shell scripts.

## Development Environment

### Building

```bash
# Build entire project
make build

# Clean build artifacts
make clean
```

### Development Environment (Docker)

```bash
# Start Docker development environment
make dev
```

### Testing

```bash
# Run all tests
make test
```

### Linting

```bash
# Run all linters (Clang-Tidy, Cppcheck)
make lint
```

## Code Style Guidelines

### File Structure

- `src/`: Main source code
- `src/ui/`: UI components (Widgets, Windows)
- `src/models/`: Data models
- `src/utils/`: Utility classes

### Conventions

- **Classes**: PascalCase (`StudioMainWindow`, `PropertyEditor`)
- **Methods**: camelCase (`setupUi`, `loadProject`)
- **Files**: PascalCase for classes (`StudioMainWindow.cpp`), lowercase for utilities
- **Qt**: Use `Q_OBJECT`, signals/slots, and proper parenting.

### Error Handling

- Use `qDebug()`, `qInfo()`, `qWarning()`, `qCritical()`
- Check pointers before use (`Q_ASSERT`, `if (ptr)`)

## Repository Structure

- `src/` - Application source code
- `tests/` - Unit tests (GoogleTest/QtTest)
- `docs/` - Documentation
- `packaging/` - Packaging scripts

## PR Guidelines

- Follow conventional commits (`feat:`, `fix:`, `docs:`)
- Ensure `make test` passes
- Ensure `make lint` passes
- Update documentation if UI changes

## AI Context References

- Documentation: `docs/`
- SHantilly Agent Guide: `../SHantilly/AGENTS.md` (if available alongside)
