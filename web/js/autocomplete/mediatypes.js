//apiurl='https://www.reseau-canope.fr/scolomfr/data/rest/v1/scolomfr-8-0'
//radicals = [ 'application/','audio/','image/','model/','multipart/','text/','video/' ]
//mediatypes=[]; get_terms_from_skosmos(apiurl, radicals, 'mediatypes', lang='fr')

mediatypes=[]; get_terms_from_bioportal('FDC-GDMT', 'http://vocab.fairdatacollective.org/gdmt/MIMEType', 'mediatypes');
