<?php

$URL=$APPURL."/metadata/$id_metadata/?format=jsonld";

?>


<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>JSON-LD Graph Viewer + Export</title>
  <script src="https://unpkg.com/vis-network/standalone/umd/vis-network.min.js"></script>
  <style>
    body { font-family: sans-serif; }
    #controls { margin: 1em 0; display: flex; gap: 2em; align-items: center; }
    #mynetwork { width: 100%; height: 800px; border: 1px solid lightgray; }
  </style>
</head>
<body>

<h2>JSON-LD Graph Viewer (<?= $id_metadata ?>)</h2>

<div id="controls">
  <div>
    <strong>View mode:</strong><br>
    <label><input type="radio" name="mode" value="simple" checked> Simple</label>
    <label><input type="radio" name="mode" value="full"> Full</label>
  </div>
  <div>
    <strong>Export:</strong><br>
    <select id="exportType">
      <option value="rdf">RDF Triples</option>
    </select>
    <button id="exportBtn">Export</button>
  </div>
</div>

<div id="mynetwork"></div>

<script>
  const container = document.getElementById('mynetwork');
  let jsonldData = null;
  let currentNetwork = null;
  let currentEdges = null;
  let currentNodes = null;

  async function fetchData() {
    const res = await fetch('<?= $URL ?>');
    jsonldData = await res.json();
    renderGraph('simple');
  }

  function renderGraph(mode = 'simple') {
    const nodes = new vis.DataSet();
    const edges = new vis.DataSet();
    const seen = new Set();
    let anonCounter = 0;

    function getNodeId(obj) {
      if (typeof obj === 'string') return obj;
      if (obj['@id']) return obj['@id'];
      if (obj['name']) return obj['name'];
      return 'anon_' + (++anonCounter);
    }

    function process(obj, parentId = null, prop = null) {
      const currentId = getNodeId(obj);

      if (!seen.has(currentId)) {
        const label = obj['name'] || obj['@id'] || obj['description'] || currentId;
        nodes.add({ id: currentId, label: label.toString(), shape: 'ellipse' });
        seen.add(currentId);
      }

      if (parentId && prop) {
        edges.add({ from: parentId, to: currentId, label: prop });
      }

      for (const key in obj) {
        if (key === '@context') continue;
        const val = obj[key];

        if (typeof val === 'object' && val !== null) {
          if (Array.isArray(val)) {
            val.forEach(v => {
              if (typeof v === 'object') {
                process(v, currentId, key);
              } else if (mode === 'full') {
                const literalId = currentId + '_' + key + '_' + v;
                if (!seen.has(literalId)) {
                  nodes.add({ id: literalId, label: `${key}: ${v}`, shape: 'box', color: '#e0e0e0' });
                  edges.add({ from: currentId, to: literalId, label: key });
                  seen.add(literalId);
                }
              }
            });
          } else {
            process(val, currentId, key);
          }
        } else if (mode === 'full' && (typeof val === 'string' || typeof val === 'number')) {
          const literalId = currentId + '_' + key + '_' + val;
          if (!seen.has(literalId)) {
            nodes.add({ id: literalId, label: `${key}: ${val}`, shape: 'box', color: '#f0f0f0' });
            edges.add({ from: currentId, to: literalId, label: key });
            seen.add(literalId);
          }
        }
      }
    }

    process(jsonldData);

    const data = { nodes, edges };
    const options = {
      layout: { improvedLayout: true },
      edges: { arrows: 'to', font: { align: 'middle' }, smooth: true },
      nodes: {
        font: { size: 14 },
        shape: 'dot',
        scaling: { label: true },
        shadow: true
      },
      physics: {
        barnesHut: {
          gravitationalConstant: -30000,
          springLength: 150
        },
        stabilization: true
      }
    };

    currentNetwork = new vis.Network(container, data, options);
    currentNodes = nodes;
    currentEdges = edges;
  }

  // Handle view mode switch
  document.querySelectorAll('input[name="mode"]').forEach(radio => {
    radio.addEventListener('change', (e) => {
      if (jsonldData) renderGraph(e.target.value);
    });
  });

  // Handle export actions
  document.getElementById('exportBtn').addEventListener('click', () => {
    const type = document.getElementById('exportType').value;
    if (type === 'rdf') {
      exportRDF();
    } else if (type === 'rdfxml') {
      exportRDFXML();
    }
  });

  function exportRDF() {
    if (!currentEdges || !currentNodes) return;

    let rdf = '';
    currentEdges.forEach(edge => {
      const subject = edge.from;
      const predicate = edge.label || 'relatedTo';
      const object = edge.to;
      rdf += `<${subject}> <${predicate}> <${object}> .\n`;
    });

    const blob = new Blob([rdf], { type: 'text/plain' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = 'graph-triples.nt';
    a.click();
    URL.revokeObjectURL(url);
  }

  fetch('<?= $URL ?>')
    .then((res) => res.json())
    .then((data) => {
        jsonldData = data;
        renderGraph('simple');
  });

</script>

</body>
</html>
