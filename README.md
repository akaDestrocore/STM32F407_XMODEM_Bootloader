# STM32 Bootloader

## Overview
This project provides a firmware bootloader for STM32, supporting the **XMODEM** protocol and using the **JANPATCH** library for applying binary patches. The system supports various firmware management operations, such as uploading new firmware patches, downloading existing firmware, and erasing existing firmware.

---

## Architecture

### Key Components
The firmware update architecture consists of the following components:
- **BOOT:** A minimal software whose sole purpose is to load the LOADER. If the LOADER is invalid, it falls back to another application.
- **APP_LOADER:** Validates and loads the application image or redirects to the UPDATER if the application is invalid.
- **SLOT_2_APPLICATION:** The main application code, which does not perform updates.
- **UPDATER:** Updates the main application and performs related operations.

### Architecture Diagram

```mermaid
graph LR
    subgraph SLOT 0
        BOOT
    end
    subgraph SLOT 1
        LOADER[APP_LOADER]
        UPDATER
    end
    subgraph SLOT 2
        Application
    end

    subgraph UPDATER
        LOADER[APP_LOADER]
    end
    
    BOOT -->|Loads| LOADER
    BOOT -.->|Loads| UPDATER
    LOADER -->|Loads| Application
    UPDATER -->|Updates| Application
    UPDATER -->|Loads| LOADER
