<?php

error_reporting(E_ERROR);
ini_set('display_errors', '0');

require 'vendor/autoload.php';

use EasyRdf\Graph;
use EasyRdf\RdfNamespace;
use EasyRdf\Format;

// Créer un graphique RDF depuis JSON-LD
$graph = new Graph();
$graph->parse($jsonld, 'jsonld');

// Convertir en RDF/XML
$rdfXml = $graph->serialise('rdfxml');

// Retourner le contenu RDF/XML
header('Content-Type: application/rdf+xml');
echo $rdfXml;

?>