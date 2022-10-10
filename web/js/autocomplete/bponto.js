// NCBITAXON : 'Solanaceae' + 'Vitis'
// bponto=[]; get_terms_from_bioportal('NCBITAXON', 'http://purl.bioontology.org/ontology/NCBITAXON/4070', 'bponto');
//            get_terms_from_bioportal('NCBITAXON', 'http://purl.bioontology.org/ontology/NCBITAXON/3603', 'bponto');

// NCIT : 'Document'
// bponto=[]; get_terms_from_bioportal('NCIT', 'http://ncicb.nci.nih.gov/xml/owl/EVS/Thesaurus.owl#C19498', 'bponto');


// EDAM : Data + Format
bponto=[]; get_terms_from_bioportal('EDAM', 'http://edamontology.org/data_0006', 'bponto');
           get_terms_from_bioportal('EDAM', 'http://edamontology.org/format_1915', 'bponto');

