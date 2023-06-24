const fs = require('fs');

fs.readFile(process.argv[2], 'utf8', (err, data) => {
  if (err) {
    console.error(err);
    return;
  }

  var output = {}

  data.split(" ").map(i => ((word) => word.split("").map(ch => (ch >= "a" && ch <= "z") ? ch : "").join(""))(i.toLowerCase())).forEach(word => isNaN(output[word]) ? output[word] = 1 : output[word]++)

  console.table(output);
});
