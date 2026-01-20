# Guia de Contribuição - SHantilly-Studio

Obrigado pelo interesse em contribuir com o SHantilly-Studio! Somos uma IDE visual para criar interfaces SHantilly e valorizamos sua ajuda.

## 🚀 Como Começar

1. **Fork** o repositório no GitHub.
2. **Clone** seu fork localmente:

    ```bash
    git clone https://github.com/SEU_USUARIO/SHantilly-Studio.git
    cd SHantilly-Studio
    ```

3. Instale as dependências e o submodúlo/core:

    ```bash
    # O Studio depende do SHantilly core, geralmente gerenciado aqui ou instalado via vcpkg
    make build
    ```

## 🛠️ Padrões de Desenvolvimento

### Estilo de Código

O código deve ser limpo e bem documentado.

- **C++**: Seguimos o arquivo `.clang-format`.
- **Qt**: Utilizamos padrões modernos do Qt6 (sinais/slots com ponteiros de função, etc).

Antes de submeter, rode:

```bash
make format
make lint
```

### Estrutura do Projeto

- `src/`: Código fonte da IDE.
- `src/ui/`: Componentes da interface gráfica.
- `src/models/`: Modelos de dados e lógica de negócio.

## 📦 Processo de Pull Request

1. Crie uma branch descritiva (`feat/...`, `fix/...`).
2. Implemente suas mudanças.
3. **Teste muito bem**: Como é uma IDE, alterações na UI podem ter efeitos colaterais visuais.
4. Submeta o PR preenchendo o template.

## 🐛 Reportando Bugs

Use o Issue Tracker do GitHub. Tente incluir:

- Passos para reproduzir.
- Log de saída do terminal.
- Capturas de tela.

## 📄 Licença

Ao contribuir, você aceita licenciar seu código sob a licença GPLv3+ do projeto.
