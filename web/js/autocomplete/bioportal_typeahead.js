let bioportal_api='https://bioportal.bioontology.org/search/json_search/?target_property=name&ontologies='
let bioportal_logo = 'https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTmvaQl_K9V8g9qv1oywrfUCjBT8_rZXtolGg&s'
//let bioportal_logo = 'https://ontoportal.org/images/logo.png'
let bioportal_limit=99

// BioPortal Search API with help of Typeahead
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
				url = bioportal_api+ontology.replace(/:/g,',')+'&q=*'+encodeURIComponent(query)+'*&response=json&callback=?'
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
				return data.label;
		}
	});
}
