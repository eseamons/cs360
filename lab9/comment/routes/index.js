var express = require('express');
var router = express.Router();

var mongoose = require('mongoose');
var Comment = mongoose.model('Comment');

router.get('/comments', function(req, res, next) {
  Comment.find(function(err, comments){
    if(err){ return next(err); }
    res.json(comments);
  });
});

router.post('/comments', function(req, res, next) {
  var comment = new Comment(req.body);
  comment.save(function(err, comment){
    if(err){ return next(err); }
    res.json(comment);
  });
});

router.param('comment', function(req, res, next, id) {
  var query = Comment.findById(id);
  query.exec(function (err, comment){
    if (err) { return next(err); }
    if (!comment) { return next(new Error("can't find comment")); }
    req.comment = comment;
    return next();
  });
});

router.get('/comments/:comment', function(req, res) {
  res.json(req.comment);
});

router.delete('/comments/:comment/delete', function(req, res) {

	console.log(req.comment);
	var id = req.comment._id;

    Comment.remove({"_id":id}, function(err, result) { 
        res.send( (result === 1) ? { msg: 'Deleted' } : { msg: 'error: '+ err } );
    });
	
});
router.put('/comments/:comment/upvote', function(req, res, next) {
  req.comment.upvote(function(err, comment){
    if (err) { return next(err); }
    res.json(comment);
  });
});

module.exports = router;
