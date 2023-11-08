document.addEventListener("DOMContentLoaded", function () {
  const loadingOverlay = document.getElementById("loading-overlay");
  const anchorTags = document.querySelectorAll("a");

  anchorTags.forEach(function (anchor) {
    anchor.addEventListener("click", function (event) {
      // Show the loading overlay
      loadingOverlay.style.display = "block";

      // Prevent the default behavior (page navigation)
      event.preventDefault();

      // Load the new page immediately
      window.location = anchor.getAttribute("href");
    });
  });

  // Detect when the new page has finished loading
  window.onload = function () {
    // Hide the loading overlay when the new page is fully loaded
    loadingOverlay.style.display = "none";
  };

  // Handle the popstate event (when using the back/forward buttons)
  window.addEventListener("popstate", function () {
    // Hide the loading overlay when navigating back to a previous page
    loadingOverlay.style.display = "none";
  });
});
