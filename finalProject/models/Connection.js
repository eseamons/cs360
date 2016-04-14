var mongoose = require('mongoose');
var ConnectionSchema = new mongoose.Schema({
  restaurant_id: String,
  user_id: String,
});

mongoose.model('Connection', ConnectionSchema);