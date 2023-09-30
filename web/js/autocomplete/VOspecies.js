apiurl='https://consultation.vocabulaires-ouverts.inrae.fr/rest/v1/thesaurus-inrae'

radicals = [ 'fruit','aceae' ]
VOspecies=[]; get_terms_from_skosmos(apiurl, radicals,'VOpecies')
