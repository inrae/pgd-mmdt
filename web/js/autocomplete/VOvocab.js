// Deprecated - Instead use VO via the typeahead mechanism (see VO_typeahead.js)
apiurl='https://consultation.vocabulaires-ouverts.inrae.fr/rest/v1/thesaurus-inrae'

radicals = [
'data','report','document','simulation','model','image','script','omics','genom',
'statistic','scientific','research','experiment','video','spatial','instrument',
'SNP','bio','nmr','mass','plant','fruit','growth','agricultur','water','drought',
'stress','factor','analysis'
]

VOvocab=[]; get_terms_from_skosmos(apiurl, radicals,'VOvocab')

