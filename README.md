# SHantilly Studio

[![CI - Studio Build and Test](https://github.com/helton-godoy/SHantilly-Studio/actions/workflows/ci.yml/badge.svg)](https://github.com/helton-godoy/SHantilly-Studio/actions/workflows/ci.yml)
[![CodeQL](https://github.com/helton-godoy/SHantilly-Studio/actions/workflows/codeql.yml/badge.svg)](https://github.com/helton-godoy/SHantilly-Studio/actions/workflows/codeql.yml)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)

**IDE Visual para criação de interfaces SHantilly** - Desenvolva GUIs profissionais para seus scripts shell através de uma interface "arrastar-e-soltar" intuitiva. Preview em tempo real e geração automática de código.

## 🚀 Início Rápido

```bash
# Clonar o repositório
git clone https://github.com/helton-godoy/SHantilly-Studio.git
cd SHantilly-Studio

# Build via Docker (recomendado)
make build

# Executar
make run
```

## 📋 Pré-requisitos

- Docker (recomendado) ou
- Qt6 (QtWidgets, QtCharts, QtSVG)
- CMake 3.16+
- C++17 compiler

## 🔧 Comandos Principais

| Comando | Descrição |
|---------|-----------|
| `make build` | Compila o projeto |
| `make run` | Executa o Studio |
| `make test` | Executa os testes |
| `make lint` | Verifica qualidade do código |
| `make format` | Formata o código |
| `make docs` | Gera documentação |
| `make help` | Lista todos os comandos |

## 📁 Estrutura do Projeto

```
SHantilly-Studio/
├── src/                    # Código-fonte principal
├── tests/                  # Testes unitários
├── docs/                   # Documentação
├── packaging/              # Scripts de empacotamento
├── .github/workflows/      # CI/CD GitHub Actions
├── CMakeLists.txt          # Build system
└── Makefile                # Automação de desenvolvimento
```

## 🔗 Dependências

Este projeto depende do [SHantilly](https://github.com/helton-godoy/SHantilly) (runtime de widgets).

## 📖 Documentação

- [Arquitetura](docs/ARCHITECTURE.md)
- [Guia de Desenvolvimento](docs/hybrid_workflow.md)

## 🤖 Para Agentes de IA

Consulte o arquivo [AGENTS.md](AGENTS.md) para instruções detalhadas sobre como contribuir com este projeto.

## 📄 Licença

Este projeto está licenciado sob a GPLv3+ - veja o arquivo [LICENSE](LICENSE) para detalhes.
