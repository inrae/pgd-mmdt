// ORCID API with help of Typeahead
// See https://info.orcid.org/documentation/api-tutorials/api-tutorial-searching-the-orcid-registry/

function get_searchTerm() {
	return $('#Cellcol0').val().toLowerCase().no_utf8()+','+$('#Cellcol1').val().toLowerCase().no_utf8();
}

function orcidTypeahead ()
{
	$('#container .ORCID').typeahead({
		minLength: 0,
		highlight: true
	},
	{
		limitget: 20,
		limitview : 9,
		async: true,
		name: 'orcid-search',
		display: function(item) {
			return item["orcid"];
		},
		source: function (query, processSync, processAsync) {
			return $.ajax({
				url: "?orcid=" + get_searchTerm(),
				type: 'GET',
				dataType: 'json',
				timeout: 3000,
				success: function (json) {
					return processAsync(json);
				},
				error: function(xhr, status) {
					if (status === 'timeout') console.warn('The waiting time has ended');
					return processAsync([]);
				}
			});
		},
		templates: {
			pending: [
				'<div class="empty-message tt-Loading">',
				'<img src="https://homepage-prod.orcid.org/assets/iD_icon_1-b2beec997a.png" style="width:50px;"/>&nbsp;&nbsp;'+
				'<b><i>Wait while fetching people list ...</i></b>',
				'</div>'
			].join('\n'),
			suggestion: function(item) {
				return '<div><strong>'+item['given-names']+' '+item['family-name']+'</strong><br><small>'+item['email']+' – '+item['current-institution-affiliation-name']+'</small></div>';
			}
		}
	});
}
