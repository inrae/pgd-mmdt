// -- ols type --

// Variables used only in this script.
let nfdi4health_api ='https://semanticlookup.zbmed.de/ols/api/search'
let nfdi4health_logo = 'https://www.nfdi4health.de/images/logo/nfdi4health.svg'
let nfdi4health_limit = 200
let nfdi4health_options  = 'exact=false&obsoletes=false&local=false'
    nfdi4health_options += '&isLeaf=false&inclusive=false&queryFields=label'
    nfdi4health_options += '&type=class&rows=200&format=json&lang=en'

// NFDI4Health OLS API with help of Typeahead
// idName : the magggot field name 
// ontology : ontology list separated by a colon (':')
var nfdi4health_typeahead = function (idName, ontology)
{
	$('#nfdi4health-'+idName+' .typeahead').typeahead({	hint: true,	highlight: true, minLength: 3 },
	{
		limitget: nfdi4health_limit,
		limitview : nfdi4health_limit,
		async: true,
		source: function (query, processSync, processAsync) {
			onto='?'
			if (ontology.length>0 && ontology != 'all') {
				items=ontology.split(':');
				items.forEach((item) => { onto = onto + 'ontology='+item.toLowerCase()+'&'; });
			}
			url = nfdi4health_api+onto+nfdi4health_options+'&start=0&q=*'+encodeURIComponent(query)+'*'
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
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="'+nfdi4health_logo+'" style="width:60px;"/>&nbsp;&nbsp;'+
				'<b><i><font color="grey" size=-1>&nbsp;'+shrink_ontolist(ontology,40)+'</font></i></b>',
				'</div>'
			].join('\n'),
			pending: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="'+nfdi4health_logo+'" style="width:60px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp;... wait ...</i></b>',
				'</div>'
			].join('\n'),
			empty: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="'+nfdi4health_logo+'" style="width:60px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp; this term seems not exist !! </i></b>',
				'</div>'
			].join('\n'),
			footer: [
				'<div class="empty-message" style="display: inline-block;">',
				'<b><i><font color="grey" size=-1>Result provided by Terminology Service for NFDI4Health</font></i></b>',
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