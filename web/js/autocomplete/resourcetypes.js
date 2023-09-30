//apiurl='http://vocab.fairdatacollective.org/rest/v1/datacite'
//parent = 'http://purl.org/datacite/v4.4/ResourceTypeGeneral'
//resourcetypes=['None']; get_terms_from_skosmos(apiurl, [], 'resourcetypes', lang='en', parent=parent)

resourcetypes=[]; get_terms_from_bioportal('DATACITE-VOCAB', 'http://purl.org/datacite/v4.4/ResourceTypeGeneral', 'resourcetypes');
