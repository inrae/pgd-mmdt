// EMBL-OLS : see https://www.ebi.ac.uk/ols4/help

var olsapi="https://www.ebi.ac.uk/ols4/api/search"

options="exact=false&obsoletes=false&local=false&rows=1000&format=json&lang=en"

query='*'
ontology='edam:efo'

onto='?'
items=ontology.split(':')
items.forEach((item) => { onto = onto + 'ontology='+item+'&'; });

function getEMBL_OLS_terms (olsapi_url) {
    $.getJSON(olsapi_url, function (data) {
        if (typeof data != 'undefined' && data.hasOwnProperty('response')) {
            list = data.response.docs;
            for (let i = 0; i < list.length; i++) embl_ols.push(list[i]['label']);
        }
    });
}

var embl_ols=[]
getEMBL_OLS_terms (olsapi+onto+options+'&start=0&q='+query)
getEMBL_OLS_terms (olsapi+onto+options+'&start=1000&q='+query)
