## Change Log


### Developments in the pipeline


* **Release 2.3** - before the end of 2025

   * Addition of a "search bar" allowing you to enter a series of keywords in order to perform a search across all fields, in addition to the advanced search by field - as currently in the Maggot application (_done_).

   * Reloading a metadata file is now possible directly from the data storage space, knowing that reloading can still be done from your own disk (_done_).

   * From the metadata sheet of a dataset, it is now possible to reload it in edit mode (_done_).

   * "Internal metadata management," i.e that the metadata will be stored in an internal space managed by the Maggot application. The implementation are still to be defined, but this internal management will have to be configured in advance as an option; it will not be possible to mix the two approaches (internal/external) (_To do_).

<br>

### Summary of past and recent changes


* **Release 2.2** - June 2025 (_current_)

   * Simplifies the terminology update process. You can now update both terminology definitions and documentation from the web interface. However, this requires administrator privileges.
   
   * Added the ability to search for an ORCID number from the last name and first name in the people dictionary. See [issue #6](https://github.com/inrae/pgd-mmdt/issues/6)

   * Proposes as an option, a SSO layer (https://github.com/djacob65/maggot-sso/wiki). As authentication are manage by the SSO layer, the authorisation are manage by Maggot.
   
   * Added a confirmation key to generate a private access key file. See [issue #3](https://github.com/inrae/pgd-mmdt/issues/3)

   * Added constraints about enumerations based on predefined terms in the JSON schema of metadata.

   * Fixed issue when scanning while some metadata files have incorrect format. See [issue #2](https://github.com/inrae/pgd-mmdt/issues/2)
   
   * It is now possible to check the version of Maggot after an installation. We can access the current version from the About tab and a file named VERSION under the root contains the version number. See [issue #5](https://github.com/inrae/pgd-mmdt/issues/5)

   * Fixed some bugs due to upgrading to PHP 8


* **Release 2.1** - September 2024

   * Unified API calls using Twitter's Typeahead library. This includes controlled vocabularies and ontologies. This makes it easy to implement calls to the Ontoportal, Skosmos, and EBI OLS APIs.

   * Added the ability to deposit metadata into the bloxberg channel (experimental).

   * Added the ability to protect your metadata sets by a password.

   * Fixes numerous small bugs.


* **Release 2.0** - September 2023

   * Integrates dictionaries of people as well as of controlled vocabulary, which can be updated from the web interface with administrator rights.
   
   * Added the ability to deposit metadata into repositories like Zenodo or based on Harvard Dataverse.

   * Added the ability to harvest metadata based on the OAI-PMH protocol.

   * Added the ability to export metadata in JSON-LD format

   * Proposes as an option, a filebrowser to install separately (https://github.com/djacob65/maggot-fb)

