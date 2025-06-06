## Summary of past and recent changes and planning of improvements and/or additions of new possibilities


* **Release 2.3** - before the end of 2025 (_planned_)

   * "Internal metadata management," i.e that the metadata will be stored in an internal space managed by the Maggot application.
       * This internal management option will be configured in advance. It will not be possible to mix the two approaches (internal/external).
       * The metadata form pre-fill feature (the "Pre-fill") will include the ability, in addition to the existing option to import an external metadata file, to use an internal metadata file if this option has been selected.
       * Important: To perform this management, it will be necessary to add a username to the metadata file to keep track of who created it. There are two scenarios, depending on whether Maggot's SSO layer is installed (https://github.com/djacob65/maggot-sso/wiki) or not:
           * The SSO layer is not installed; in this case, the default username will be "anonymous." And therefore, anyone will be able to modify or overwrite the metadata. So not highly recommended.
           * The SSO layer is installed and then the "username" will be the login name. Only the user who created the metadata and anyone with "admin" rights will be able to modify the metadata.

   * Addition of a "search bar" allowing you to enter a series of keywords in order to perform a search across all fields, in addition to the advanced search by field - as currently in the Maggot application.


* **Release 2.2** - June 2025 (_current_)

   * Simplifies the terminology update process. You can now update both terminology definitions and documentation from the web interface. However, this requires administrator privileges.
   
   * Adds the ability to search for an ORCID number from the last name and first name in the people dictionary.

   * Proposes as an option, a Maggot's SSO layer (https://github.com/djacob65/maggot-sso/wiki). As authentication are manage by the SSO layer, the authorisation are manage by Maggot.


* **Release 2.1** - September 2024

   * Unified API calls using Twitter's Typeahead library. This includes controlled vocabularies and ontologies. This makes it easy to implement calls to the Ontoportal, Skosmos, and EBI OLS APIs.

   * Adds the ability to deposit metadata into the bloxberg channel (experimental).

   * Adds the ability to protect your metadata sets by a password.

   * Fixes numerous small bugs.


* **Release 2.0** - September 2023

   * Adds dictionaries
   
   * Adds the ability to deposit metadata into repositories like Zenodo or based on Harvard Dataverse.

   * Proposes as an option, a filebrowser to install separately (https://github.com/djacob65/maggot-fb)

