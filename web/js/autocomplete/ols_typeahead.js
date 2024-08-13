// Variables used only in this script.
let ols_api ='https://www.ebi.ac.uk/ols4/api/search'
let ols_logo = 'https://www.ebi.ac.uk/ols4/logo.svg'
let ols_limit = 99
let ols_options  = 'exact=false&obsoletes=false&local=false'
    ols_options += '&isLeaf=false&inclusive=false&queryFields=label'
    ols_options += '&type=class&rows=200&format=json&lang=en'

// EBI OLS API with help of Typeahead
// idName : the magggot field name 
// ontology : ontology list separated by a colon (':')
var ols_typeahead = function (idName, ontology)
{
	$('#ols-'+idName+' .typeahead').typeahead({	hint: true,	highlight: true, minLength: 3 },
	{
		limitget: ols_limit,
		limitview : ols_limit,
		async: true,
		source: function (query, processSync, processAsync) {
			onto='?'
			if (ontology.length>0 && ontology != 'all') {
				items=ontology.split(':');
				items.forEach((item) => { onto = onto + 'ontology='+item.toLowerCase()+'&'; });
			}
			url = ols_api+onto+ols_options+'&start=0&q=*'+encodeURIComponent(query)+'*'
			if (DEBUG) console.log('GET '+url)
			return $.ajax({
				url: url,
				type: 'GET',
				dataType: 'json',
				success: function (json) {
					return processAsync([...new Map(json.response.docs.map(item => [item.label, item])).values()]);
				}
			});
		},
		templates: {
			header: [
				// EBI Logo : https://avatars.githubusercontent.com/u/31919308?v=4
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="'+ols_logo+'" style="width:30px;"/>&nbsp;&nbsp;'+
				'<b><i><font color="grey" size=-1>&nbsp;'+shrink_ontolist(ontology,40)+'</font></i></b>',
				'</div>'
			].join('\n'),
			pending: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="'+ols_logo+'" style="width:30px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp;... wait ...</i></b>',
				'</div>'
			].join('\n'),
			empty: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="'+ols_logo+'" style="width:30px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp; this term seems not exist !! </i></b>',
				'</div>'
			].join('\n'),
			footer: [
				'<div class="empty-message" style="display: inline-block;">',
				'<b><i><font color="grey" size=-1>Result provided by EMBL-EBI Ontology Lookup Service</font></i></b>',
				'</div>'
			].join('\n'),
			suggestion: function (data) {
				return '<p>'+data.label+'&nbsp;<font color=grey><i>('+data.ontology_prefix+')</i></font></p>';
			}
		},
		display: function (data) {
			return data.label;
		},
		value: function(data) {
			return data.label;
		}
	});
}