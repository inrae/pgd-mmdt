// -- ontoportal type --

// NOTE : You can copy this script to use it for another OntoPortal site (cf https://ontoportal.org/).
// Simply 1) change all 'bioportal' occurrences to the name of the new portal; 2) change the URL according to the new portal.

// Variables used only in this script.
let bioportal_api='https://data.bioontology.org'
let bioportal_apikey = 'f709257f-06a4-450c-a21f-89727cc7fbe1'
let bioportal_logo = 'https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTmvaQl_K9V8g9qv1oywrfUCjBT8_rZXtolGg&s'
let bioportal_limit=99

// BioPortal API with help of Typeahead
// idName : the magggot field name
// ontology : name(s) of the ontology
var bioportal_typeahead = function (idName, ontology)
{
	$('#bioportal-'+idName+' .typeahead').typeahead({ hint: true, highlight: true, minLength: 3 },
	{
		limitget: bioportal_limit,
		limitview : bioportal_limit,
		async: true,
		source: function (query, processSync, processAsync) {
			let params = new URLSearchParams({
				q: query,
				suggest: 'true',
				include: 'prefLabel'
			})
			if (ontology != 'all')
				params.set('ontologies', ontology.replace(/:/g, ','))

			let url = bioportal_api + '/search?' + params.toString()
			if (DEBUG) console.log('GET ' + url)

			$.ajax({
				url: url,
				type: 'GET',
				dataType: 'json',
				headers: {
					'Authorization': 'apikey token=' + bioportal_apikey
				},
				success: function (json) {
					let res = (json.collection || []).map(el => ({
						label: el.prefLabel,
						ontology: (el.links && el.links.ontology || '').split('/').pop()
					}));
					processAsync(res);
				},
				error: function (xhr) {
					console.error('BioPortal error', xhr.status);
					processAsync([]);
				}
			});
		},
		templates: {
			header: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="'+bioportal_logo+'" style="width:30px;"/>&nbsp;&nbsp;'+
				'<b><i><font color="grey" size=-1>&nbsp;'+shrink_ontolist(ontology,40)+'</font></i></b>',
				'</div>'
			].join('\n'),
			pending: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="'+bioportal_logo+'" style="width:30px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp;... wait ...</i></b>',
				'</div>'
			].join('\n'),
			empty: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="'+bioportal_logo+'" style="width:30px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp; this term seems not exist !! </i></b>',
				'</div>'
			].join('\n'),
			footer: [
				'<div class="empty-message" style="display: inline-block;">',
				'<b><i><font color="grey" size=-1>Result provided by BioPortal NCBO</font></i></b>',
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
			return data.label; // +'|'+data.ontology;
		}
	});
}
