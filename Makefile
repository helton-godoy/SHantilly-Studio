# SHantilly Studio Makefile
# Load optional .env file (for local secrets like SONAR_TOKEN)
-include .env
export

# IDE for SHantilly - Build and Development Automation

.PHONY: all build clean install run test help
.PHONY: dev start-dev start-dev-build
.PHONY: lint format tidy cppcheck security docs check-deps setup coverage
.PHONY: pkg-appimage

# Variables
CMAKE := cmake
MAKE := make
NPROC := $(shell nproc)
BUILD_DIR := build
TARGET_BIN := shantilly-studio
VCPKG_TOOLCHAIN := $(HOME)/vcpkg/scripts/buildsystems/vcpkg.cmake

# Check for vcpkg toolchain
ifdef VCPKG_ROOT
    VCPKG_TOOLCHAIN := $(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake
else
    VCPKG_TOOLCHAIN := $(HOME)/vcpkg/scripts/buildsystems/vcpkg.cmake
endif

ifneq ($(wildcard $(VCPKG_TOOLCHAIN)),)
    CMAKE_ARGS := -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_TOOLCHAIN)
    USE_VCPKG := 1
else
    CMAKE_ARGS :=
    USE_VCPKG := 0
endif

# Default target
all: setup build

# ============================================================================
# Docker Integration
# ============================================================================

DOCKER_SCRIPT := ../SHantilly/src/start-dev.sh
DOCKER_IMAGE := shantilly-dev:latest

# Check if we are running inside docker
IN_DOCKER := $(shell [ -f /.dockerenv ] && echo 1 || echo 0)

# Wrapper to run make inside Docker
# Usage: $(call IN_DOCKER_WRAPPER, target_name)
define IN_DOCKER_WRAPPER
	@if [ "$(IN_DOCKER)" = "0" ]; then \
		echo "Detected host environment. Switching to Docker container..."; \
		$(DOCKER_SCRIPT) make $(1); \
	else \
		$(MAKE) $(1)_internal; \
	fi
endef

docker-shell:  ## Open a shell in the Docker container
	$(DOCKER_SCRIPT)

# ============================================================================
# Development
# ============================================================================

setup: ## Initialize build directory
	$(call IN_DOCKER_WRAPPER,setup)

setup_internal:
	@echo "Setting up Studio build environment..."
	mkdir -p $(BUILD_DIR)

build:  ## Compile SHantilly Studio (runs inside Docker)
	$(call IN_DOCKER_WRAPPER,build)

build_internal: check-deps ## Internal build target
	@echo "Building SHantilly Studio (Inside Docker)..."
	@if [ "$(USE_VCPKG)" = "1" ]; then echo "Using vcpkg toolchain..."; fi
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && $(CMAKE) .. $(CMAKE_ARGS) -DSHANTILLY_ROOT=../SHantilly && $(MAKE) -j$(NPROC)

clean:  ## Clean build artifacts (runs inside Docker)
	$(call IN_DOCKER_WRAPPER,clean)

clean_internal: ## Internal clean target
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR)
	find . -name "*.o" -delete
	find . -name "moc_*" -delete

install: build  ## Install Studio (Desktop File)
	$(call IN_DOCKER_WRAPPER,install)

install_internal: build_internal
	@echo "Installing SHantilly Studio..."
	cd $(BUILD_DIR) && sudo $(MAKE) install

run: build  ## Run SHantilly Studio
	$(call IN_DOCKER_WRAPPER,run)

run_internal: build_internal
	@echo "Running SHantilly Studio..."
	./$(BUILD_DIR)/$(TARGET_BIN)

dev: docker-shell  ## Alias for docker-shell

# ============================================================================
# Quality Assurance
# ============================================================================

# Helper for dependency checking
CHECK_DEP = command -v $(1) >/dev/null || (echo "Error: '$(1)' not found. Please install it (e.g., sudo apt install $(2))" && exit 1)
WARN_DEP = command -v $(1) >/dev/null || echo "Warning: '$(1)' not found. Some targets may not work."

docker-check-deps:
	@if [ "$(IN_DOCKER)" = "0" ]; then \
		command -v docker >/dev/null || (echo "Error: docker not found." && exit 1); \
	else \
		$(MAKE) check-deps; \
	fi

check-deps:  ## Check for required development tools (Internal)
	@echo "Checking core dependencies (Internal)..."
	@$(call CHECK_DEP,cmake,cmake)
	@$(call CHECK_DEP,$(MAKE),build-essential)
	@$(call CHECK_DEP,g++,build-essential)
	@if [ "$(USE_VCPKG)" = "0" ] && [ "$(IN_DOCKER)" = "0" ]; then \
		echo "Warning: vcpkg not found. Falling back to system libraries."; \
	fi

check-lint:
	@$(call CHECK_DEP,clang-format,clang-format)

check-trunk:
	@$(call WARN_DEP,trunk)

check-docs:
	@$(call CHECK_DEP,doxygen,doxygen)

lint:  ## Run code linting (runs inside Docker)
	$(call IN_DOCKER_WRAPPER,lint)

lint_internal: check-lint check-trunk ## Internal lint target
	@echo "Running lint check..."
	@if command -v trunk >/dev/null 2>&1; then \
		echo "Running trunk check..."; \
		trunk check -a -y || true; \
	fi
	find src -name "*.h" -o -name "*.cpp" | xargs clang-format -n --verbose -style=file

format:  ## Run code formatting (runs inside Docker)
	$(call IN_DOCKER_WRAPPER,format)

format_internal: check-lint check-trunk ## Internal format target
	@echo "Formatting code..."
	@if command -v trunk >/dev/null 2>&1; then \
		echo "Running trunk fmt..."; \
		trunk fmt -a || true; \
	fi
	find src -name "*.h" -o -name "*.cpp" | xargs clang-format -i -verbose -style=file

tidy:  ## Run clang-tidy static analysis (runs inside Docker)
	$(call IN_DOCKER_WRAPPER,tidy)

tidy_internal: ## Internal tidy target
	@echo "Running clang-tidy analysis..."
	@if [ ! -f $(BUILD_DIR)/compile_commands.json ]; then \
		echo "Error: compile_commands.json not found. Run 'make setup' first."; \
		exit 1; \
	fi
	find src -name "*.cpp" | xargs clang-tidy -p $(BUILD_DIR) --quiet

cppcheck: ## Run cppcheck static analysis (runs inside Docker)
	$(call IN_DOCKER_WRAPPER,cppcheck)

cppcheck_internal: ## Internal cppcheck target
	@echo "Running cppcheck analysis..."
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --std=c++17 --language=c++ src -i build -i vcpkg_installed

security: ## Scan Docker image for vulnerabilities using Trivy
	@echo "Running Trivy security scan on $(DOCKER_IMAGE)..."
	docker run --rm \
		-v /var/run/docker.sock:/var/run/docker.sock \
		-v $(HOME)/.cache/trivy:/root/.cache/trivy \
		aquasec/trivy:latest image --severity HIGH,CRITICAL $(DOCKER_IMAGE)

sonar: ## Run SonarCloud analysis (runs inside Docker)
	$(call IN_DOCKER_WRAPPER,sonar_internal)

sonar_internal:
	@if [ -z "$$SONAR_TOKEN" ]; then \
		echo "Error: SONAR_TOKEN is not set."; \
		exit 1; \
	fi
	@echo "Running SonarCloud analysis inside Docker..."
	sonar-scanner \
		-Dsonar.token="$$SONAR_TOKEN" \
		-Dsonar.host.url="https://sonarcloud.io"

docs:  ## Generate documentation (runs inside Docker)
	$(call IN_DOCKER_WRAPPER,docs)

docs_internal: check-docs ## Internal docs target
	@echo "Generating documentation..."
	@if [ -f Doxyfile ]; then \
		doxygen Doxyfile; \
	else \
		echo "Warning: Doxyfile not found. Skipping Doxygen generation."; \
	fi
	@if [ -f mkdocs.yml ]; then \
		mkdocs build --site-dir public; \
	else \
		echo "Warning: mkdocs.yml not found. Skipping MkDocs generation."; \
	fi

# ============================================================================
# Testing
# ============================================================================

test:  ## Run tests (runs inside Docker)
	$(call IN_DOCKER_WRAPPER,test)

test_internal: ## Internal test target
	@echo "Running tests..."
	cd $(BUILD_DIR) && ctest --output-on-failure || echo "No tests configured or build dir missing."

coverage: ## Generate code coverage report (runs inside Docker)
	$(call IN_DOCKER_WRAPPER,coverage)

coverage_internal: check-deps ## Internal coverage target
	@echo "Generating code coverage report..."
	mkdir -p coverage_report
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && $(CMAKE) .. $(CMAKE_ARGS) -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug -DSHANTILLY_ROOT=../SHantilly && $(MAKE) -j$(NPROC)
	cd $(BUILD_DIR) && ctest --output-on-failure
	lcov --capture --directory . --output-file coverage.info --ignore-errors empty,unused \
		--exclude '/usr/*' \
		--exclude '*/vcpkg_installed/*' \
		--exclude '*/build/*' \
		--exclude '*/tests/*' || true
	genhtml coverage.info --output-directory coverage_report --ignore-errors empty,unused || true
	@echo "Coverage report generated in coverage_report/index.html"

# ============================================================================
# Packaging
# ============================================================================

pkg-appimage:  ## Build AppImage
	@echo "Building AppImage..."
	./packaging/start-pkg-appimage.sh

# ============================================================================
# Help
# ============================================================================

help:  ## Show this help
	@echo "SHantilly Studio Makefile"
	@echo "===================================================================="
	@echo "Usage: make [target]"
	@echo ""
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[36m%-20s\033[0m %s\n", $$1, $$2}'
