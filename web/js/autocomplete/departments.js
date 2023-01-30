var departments=[];
$.getJSON("https://geo.api.gouv.fr/departements", function (data) {
    $.each(data, function (index, value) {departments.push(value['nom']); });
});
