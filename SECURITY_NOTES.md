# Security and publication checklist

- Wi-Fi SSID/password values from the original firmware were replaced with placeholders.
- Firebase web configuration values in the dashboard were replaced with placeholders so the repository can be reused with a separate Firebase project.
- Firebase user credentials were not added to the public package; configure them locally and never commit them.
- Firebase project deployment metadata and the original Firebase cache were excluded.
- Original PDFs, videos, image assets, and PLC program file were excluded because they may contain personal, customer, factory, or project-specific information.
- The legacy local HTTP endpoint has no authentication in the original sketch. Do not expose it publicly.
- Before using this project with any real installation, review Firebase Rules, authentication, register mapping, and network exposure.
