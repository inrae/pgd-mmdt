// Variables used only in this script.
let ols_api="https://www.ebi.ac.uk/ols4/api/search"
let ols_limit=99
let ols_options="exact=false&obsoletes=false&local=false&isLeaf=false&inclusive=false&queryFields=label&type=class&rows=200&format=json&lang=en"

let ols_shrink = function(str, len) {
	str_shrinked=str
	if (str.length>len) {
		for (var P=[],i=1;i++,i<=str.length;) if (str[i]==":") P.push(i);  
		for (var p=0,i=1;i++,i<=P.length;) if (P[i]<len) p=i;
		str_shrinked = str.substring(0,P[p]+1)
		if (str_shrinked.length<str.length) str_shrinked += ' ...'
	} else if (str.length==0)
		str_shrinked ='all'
	return str_shrinked
}

// EBI OLS Search API with help of Typeahead
// this function must be named as <ws>_typeahead
// idName : the magggot field name 
// ontology : ontology list separated by a colon (':')
var ols_typeahead = function (idName, ontology)
{
	$('#ols-'+idName+' .typeahead').typeahead({
		hint: true,
		highlight: true,
		minLength: 3
	},
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
				'<img src="https://www.ebi.ac.uk/ols4/logo.svg" style="width:30px;"/>&nbsp;&nbsp;'+
				'<b><i><font color="grey" size=-1>&nbsp;'+ols_shrink(ontology,40)+'</font></i></b>',
				'</div>'
			].join('\n'),
			pending: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="https://www.ebi.ac.uk/ols4/logo.svg" style="width:30px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp;... wait ...</i></b>',
				'</div>'
			].join('\n'),
			empty: [
				'<div class="empty-message" style="display: inline-block;">',
				'<img src="https://www.ebi.ac.uk/ols4/logo.svg" style="width:30px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp; this term seems not exist !! </i></b>',
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