var mongoose = require('mongoose');
var RestaurantSchema = new mongoose.Schema({
  name: String,
  address: String,
  country_id: String,
  connections: {type: Number, default: 0},
});

/*
RestaurantSchema.methods.addConnection = function(cb) {
  this.connections += 1;
  this.save(cb);
};
*/
mongoose.model('Restaurant', RestaurantSchema);