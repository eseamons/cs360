angular.module('comment', [])
.controller('MainCtrl', [
  '$scope','$http', '$window', 
  function($scope,$http,$window){
    $scope.test = 'Hello world!';

    $scope.getAll = function() {
	    return $http.get('/comments').success(function(data){
	      angular.copy(data, $scope.comments);
	    });
	};


	$scope.comments = [];
    $scope.addComment = function() {
	      if($scope.formContent === '') { return; }
	      console.log("In addComment with "+$scope.formContent);
	      $scope.create({
	        title: $scope.formContent,
	        upvotes: 0,
	      });
	      $scope.formContent = '';
    };

    $scope.create = function(comment) {
	    return $http.post('/comments', comment).success(function(data){
	      $scope.comments.push(data);
	    });
    };

    $scope.incrementUpvotes = function(comment) {
      $scope.upvote(comment);
    };

    $scope.upvote = function(comment) {
      return $http.put('/comments/' + comment._id + '/upvote')
        .success(function(data){
          console.log("upvote worked");
          comment.upvotes += 1;
        });
    };

    $scope.removeComment = function(comment) {

    	var decision = $window.confirm('Are you sure you want to delete this comment? If so, click Ok.');
    	if(decision) {
    		$scope.remove(comment);
    	}
    }


    $scope.remove = function(comment) {
    	var index = $scope.comments.indexOf(comment);
        return $http.delete('/comments/' + comment._id + '/delete')
        .success(function(data){
          console.log("delete worked");
          $scope.comments.splice(index, 1);
        });
    }

    angular.element(document).ready($scope.getAll());

  }
]);