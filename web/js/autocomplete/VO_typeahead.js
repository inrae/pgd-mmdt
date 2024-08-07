// Mandatory variable : <ws>_ontology 
// it will be initialized with the 'onto' parameter specified in the 'festures' column of the 'config_term.txt' configuration file.
var VO_ontology='thesaurus-inrae'

// other variables used only in this script.
var VO_api='https://consultation.vocabulaires-ouverts.inrae.fr/rest/v1/'
var VO_options='lang=en&type=skos:Concept&parent=&group='


// VOINRAE Search API with help of Typeahead
// this function must be named as <ws>_typeahead
function VO_typeahead () {
	$('#VO .typeahead').typeahead({
		hint: true,
		highlight: true,
		minLength: 3
	},
	{
		limitget: 50,
		limitview : 30,
		async: true,
		source: function (query, processSync, processAsync) {
			url = VO_api+VO_ontology+'/search?'+VO_options+'&query='+query+'*'
			return $.ajax({
				url: url,
				type: 'GET',
				dataType: 'json',
				success: function (json) {
					return processAsync(json.results);
				}
			});
		},
		templates: {
			header: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="https://vocabulaires-ouverts.inrae.fr/wp-content/uploads/sites/50/2022/09/cropped-logo-VOINRAE-267x66.png" style="width:60px;"/>&nbsp;&nbsp;'+
				'<b><i><font color="grey" size=-1>&nbsp;'+VO_ontology+'</font></i></b>',
				'</div>'
			].join('\n'),
			pending: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="https://vocabulaires-ouverts.inrae.fr/wp-content/uploads/sites/50/2022/09/cropped-logo-VOINRAE-267x66.png" style="width:60px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp;... wait ...</i></b>',
				'</div>'
			].join('\n'),
			empty: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="https://vocabulaires-ouverts.inrae.fr/wp-content/uploads/sites/50/2022/09/cropped-logo-VOINRAE-267x66.png" style="width:60px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp; this term seems not exist !! </i></b>',
				'</div>'
			].join('\n'),
			suggestion: function (data) {
				return '<p>'+data.prefLabel+'</p>';
			}
		},
		display: function (data) {
			return data.prefLabel;
		},
		value: function(data) {
			return data.prefLabel;
		}
	});
}