$(document).ready(function() {
	
    /* ===== Stickyfill ===== */
    /* Ref: https://github.com/wilddeer/stickyfill */
    // Add browser support to position: sticky
    var elements = $('.sticky');
    Stickyfill.add(elements);


    /* Activate scrollspy menu */
    $('body').scrollspy({target: '#doc-menu', offset: 100});
    
    /* Smooth scrolling */
	$('a.scrollto').on('click', function(e){
        //store hash
        var target = this.hash;    
        e.preventDefault();
		$('body').scrollTo(target, 800, {offset: 0, 'axis':'y'});
		
	});
     
    /* Bootstrap lightbox */
    /* Ref: http://ashleydw.github.io/lightbox/ */

    $(document).delegate('*[data-toggle="lightbox"]', 'click', function(e) {
        e.preventDefault();
        $(this).ekkoLightbox();
    });    


});


function copyCode() {
    const codeElement = document.getElementById("codeToCopy");
    const codeText = codeElement.innerText;
    
    navigator.clipboard.writeText(codeText).then(() => {
        alert("Code copied to clipboard!");
    }).catch(err => {
        console.error("Failed to copy: ", err);
    });
}
document.getElementById("downloadLinksh").addEventListener("click", function(event) {
    event.preventDefault();
    const url = this.href;
    const fileName = "installer.sh";

    fetch(url)
      .then(response => response.blob())
      .then(blob => {
        const link = document.createElement("a");
        link.href = window.URL.createObjectURL(blob);
        link.download = fileName;
        link.click();
      })
      .catch(console.error);
  });
document.getElementById("downloadLinkbat").addEventListener("click", function(event) {
    event.preventDefault();
    const url = this.href;
    const fileName = "installer.bat";

    fetch(url)
    .then(response => response.blob())
    .then(blob => {
        const link = document.createElement("a");
        link.href = window.URL.createObjectURL(blob);
        link.download = fileName;
        link.click();
    })
    .catch(console.error);
});