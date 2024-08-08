// Variables used only in this script.
let VO_api='https://consultation.vocabulaires-ouverts.inrae.fr/rest/v1/'
let VO_options='lang=en&type=skos:Concept&parent=&group='
let VO_limit=99

// VOINRAE Search API with help of Typeahead
// this function must be named as <ws>_typeahead
// idName : the magggot field name 
// thesaurus : name of the thesaurus
var VO_typeahead = function (idName, thesaurus)
{
	$('#VO-'+idName+' .typeahead').typeahead({
		hint: true,
		highlight: true,
		minLength: 3
	},
	{
		limitget: VO_limit,
		limitview : VO_limit,
		async: true,
		source: function (query, processSync, processAsync) {
			url = VO_api+thesaurus+'/search?'+VO_options+'&query='+encodeURIComponent(query)+'*'
			if (DEBUG) console.log('GET '+url)
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
				'<b><i><font color="grey" size=-1>&nbsp;'+thesaurus+'</font></i></b>',
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