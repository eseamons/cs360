var mongoose = require('mongoose');

var CountrySchema = new mongoose.Schema({
  name: String,
});

mongoose.model('Country', CountrySchema);