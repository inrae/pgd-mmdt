var regions=[];
$.getJSON("https://geo.api.gouv.fr/regions", function (data) {
    $.each(data, function (index, value) { regions.push(value['nom']); });
});
