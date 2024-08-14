// -- ontoportal type --

// NOTE : You can copy this script to use it for another OntoPortal site (cf https://ontoportal.org/).
// Simply 1) change all 'agroportal' occurrences to the name of the new portal; 2) change the URL according to the new portal.

// Variables used only in this script.
let agroportal_url='https://agroportal.lirmm.fr'
let agroportal_logo = 'https://ontoportal.org/images/logo.png'
let agroportal_options='/search/json_search/?target_property=name&ontologies='
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
			if (ontology.length>0 && ontology != 'all')  ontology='';
			url = agroportal_url+agroportal_options+ontology.replace(/:/g,',')+'&q=*'+encodeURIComponent(query)+'*&response=json&callback=?'
			if (DEBUG) console.log('GET '+url)
			return $.ajax({
				url: url,
				type: 'GET',
					dataType: 'json',
					success: function (json) {
						res=[];
						arr = json.data.split(/~.~/);
						arr.forEach((el) => res.push(JSON.parse('{"label":"'+el.split('|')[0]+'", "ontology":"'+el.split('|')[3]+'"}' )));
						return processAsync(res);
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
