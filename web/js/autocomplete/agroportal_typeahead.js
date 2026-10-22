// -- ontoportal type --

// NOTE : You can copy this script to use it for another OntoPortal site (cf https://ontoportal.org/).
// Simply 1) change all 'agroportal' occurrences to the name of the new portal; 2) change the URL according to the new portal.

// Variables used only in this script.
let agroportal_api='https://data.agroportal.eu'
let agroportal_apikey='95ebe7f5-f8d1-41ea-a1d6-08e194196517'
let agroportal_logo = 'https://ontoportal.org/images/logo.png'
let agroportal_limit=99

// AgroPortal API with help of Typeahead
// idName : the magggot field name
// ontology : name(s) of the ontology
var agroportal_typeahead = function (idName, ontology)
{
	$('#agroportal-'+idName+' .typeahead').typeahead({ hint: true, highlight: true, minLength: 3 },
	{
		limitget: agroportal_limit,
		limitview : agroportal_limit,
		async: true,
		source: function (query, processSync, processAsync) {
			let params = new URLSearchParams({
				q: query,
				suggest: 'true',
				include: 'prefLabel'
			})
			if (ontology != 'all')
				params.set('ontologies', ontology.replace(/:/g, ','))

			let url = agroportal_api + '/search?' + params.toString()
			if (DEBUG) console.log('GET ' + url)

			$.ajax({
				url: url,
				type: 'GET',
				dataType: 'json',
				headers: {
					'Authorization': 'apikey token=' + agroportal_apikey
				},
				success: function (json) {
					let res = (json.collection || []).map(el => ({
						label: el.prefLabel,
						ontology: (el.links && el.links.ontology || '').split('/').pop()
					}));
					processAsync(res);
				},
				error: function (xhr) {
					console.error('AgroPortal error', xhr.status);
					processAsync([]);
				}
			});
		},
		templates: {
			header: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="'+agroportal_logo+'" style="width:30px;"/>&nbsp;&nbsp;'+
				'<b><i><font color="grey" size=-1>&nbsp;'+shrink_ontolist(ontology,40)+'</font></i></b>',
				'</div>'
			].join('\n'),
			pending: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="'+agroportal_logo+'" style="width:30px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp;... wait ...</i></b>',
				'</div>'
			].join('\n'),
			empty: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="'+agroportal_logo+'" style="width:30px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp; this term seems not exist !! </i></b>',
				'</div>'
			].join('\n'),
			footer: [
				'<div class="empty-message" style="display: inline-block;">',
				'<b><i><font color="grey" size=-1>Result provided by AgroPortal LIRMM</font></i></b>',
				'</div>'
			].join('\n'),
			suggestion: function (data) {
				return '<p>'+data.label+'&nbsp;<font color=grey><i>('+data.ontology+')</i></font></p>';
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
