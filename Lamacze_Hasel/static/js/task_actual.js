
function refreshGrid($grid, newdata) {
    $grid.jqGrid('clearGridData')
        .jqGrid('setGridParam', { datastr: newdata, datatype: "jsonstring"})
        .trigger('reloadGrid', [{ page: 1}]);
}

function refreshTaskActualView()
{
    $.ajax({
        url: "/task_actual",
        type: "GET",
        success: function(data) {
            var content = $(data);
            var newData = content.find("#gridData").text();
            refreshGrid($("#task_actual_tab"), newData);
        }
    });
}

function grid() {
    var tab = $("#task_actual_tab");
    var data = $("#gridData").text();
    tab.jqGrid({ url: "/task_actual",
            datatype: "jsonstring",
            datastr: data,
            mtype: "GET",
            colNames: ["", "Id","Status", "Klaster", "Data utworzenia", "Data zakończenia"],

            colModel: [ { name:'action',index:'action',sortable:false, width: 80, search:false, formatter: displayButtons },
                        { name: "id", width: 200, sorttype: "int",  align: "center", resizable: true, search:false},
                        { name: "status", width: 200, sorttype: "int", align: "center", resizable: true,sortable: true, searchoptions: {sopt: ['cn','nc','eq']}},
                        { name: "cluster", width: 200, sorttype: "text",  align: "center", resizable: true, sortable: true, searchoptions: {sopt: ['cn','nc','eq']}},
                        { name: "creation_date", width: 200, sorttype: "text",  align: "center", resizable: true, sortable: true, searchoptions: {sopt: ['cn','nc','eq']}},
                        { name: "end_time", width: 200, sorttype: "text",  align: "center", resizable: true, sortable: true, searchoptions: {sopt: ['cn','nc','eq']}}
            ],
            caption: "Aktualne zadania",
            pager: "#task_actual_pager",
            gridResize : true,
            rowNum: 10,
            rowList: [10,20,30],
            width: '100%',
            emptyrecords: "Nothing to display",
            toolbar: true,
            height: '100%',
            sortable:true,
            sortname: "id",
            sortorder: "desc",
            viewrecords: true,
            gridview: true,
            autoencode: true,
            jsonReader: {repeatitems: false, id: "0"},
            subGrid: true,
		subGridRowExpanded: function(subgrid_id, row_id) {
            var rowData = $(this).getRowData(row_id);
			var subgrid_table_id = subgrid_id+"_t";
			jQuery("#"+subgrid_id).html("<table id='"+subgrid_table_id+"' class='scroll'></table>");
			jQuery("#"+subgrid_table_id).jqGrid({
				url:"/get_passwords/" + rowData['id'],
				datatype: "json",
				colNames: ["","Id", "Algorytm", "Metoda Rozwiązania", "Status", "Hash", "Hasło", "Data Rozpoczęcia", "Data Zakończenia"],
				colModel: [
                    { name:'action',index:'action',sortable:false, width: 68, search:false, formatter: displayDetails },
					{ name: "id", width: 55, sorttype: "int", key: true, align: "center", resizable: true, sortable: true, search:false},
					{ name: "algorithm", width: 110 , sorttype: "text", align: "center",  resizable: true, sortable: true, searchoptions: {sopt: ['cn','nc','eq']}},
                    { name: "password_cracking_algorithm", width: 110 , sorttype: "text", align: "center",  resizable: true, sortable: true, searchoptions: {sopt: ['cn','nc','eq']}},
                    { name: "status", width: 55 , sorttype: "text", align: "center",  resizable: true, sortable: true, searchoptions: {sopt: ['cn','nc','eq']}},
					{ name: "hash", width: 400, align: "right", align: "center", resizable: true, sortable: true, searchoptions: { sopt: ['cn','nc','eq']} },
					{ name: "password", width: 200, align: "right", align: "center", resizable: true, sortable: true, searchoptions: {sopt: ['cn','nc','eq']}},
					{ name: "start_time", width: 150, align: "right", align: "center", resizable: true, sortable: true, searchoptions: {sopt: ['cn','nc','eq'] }},
					{ name: "end_time", width: 150, align: "right", align: "center", resizable: true, sortable: true, searchoptions: {sopt: ['cn','nc','eq']}}
				],

                loadonce: true,
                sortable: true,
                repeatitems: true,
                toolbar: true,
				height: '100%',
                width: '100%',
                emptyrecords: "Nothing to display",
				rowNum:100,
                sortname: "id",
                sortorder: "asc",
                viewrecords: true,
                gridview: true,
                autoencode: true

			});
            $("#"+subgrid_table_id).jqGrid('filterToolbar', {
            autosearch: true,
            stringResult: true,
            searchOnEnter: false,
            defaultSearch: 'cn',
            searchOperators: true} );
		}

            });
    tab.jqGrid('filterToolbar', {
            autosearch: true,
            stringResult: true,
            searchOnEnter: false,
            defaultSearch: 'cn',
            searchOperators: true} );
    tab.jqGrid('navGrid', "#task_actual_pager",
     {del:false,add:false,edit:false}, //options
    {height:280,reloadAfterSubmit:false}, // edit options
    {height:280,reloadAfterSubmit:false}, // add options
    {reloadAfterSubmit:false}) // del options)
}
function displayDetails(cellvalue, options, rowObject)
{
	var container = $("<div></div>");

	var btn = $("<button type='button' class='password_actual_details'>Szczegóły</button>");

	btn.val(JSON.stringify(rowObject));
	container.append(btn);
	return container.html();
}

function displayButtons(cellvalue, options, rowObject)
{
	var container = $("<div></div>");

	var btn = $("<button type='button' class='task_actual_details'>Szczegóły</button>");

	btn.val(JSON.stringify(rowObject));
	container.append(btn);
	return container.html();
}

$(document).ready( function(){
    grid();

      $(document).on("click", ".task_actual_details", function(e){
        e.preventDefault();
        var row_content = $(this).val();
        var row_content_json = JSON.parse(row_content);
        var url = '/task_details/' + row_content_json.id + "/"
        var d = $("<div class='warning'><iframe width='100%' height='100%' src="+ url + "></iframe></div>");
                d.dialog({
                        width : 500,
                        height : 400,
                        buttons: [
                            {
                              text: "Ok",
                              icons: {
                                primary: "ui-icon-heart"
                              },
                              click: function() {
                                $( this ).dialog( "destroy" );
                              }
                            }
                          ]
                    });


        e.preventDefault();

    });

    $(document).on("click", ".password_actual_details", function(e){
        e.preventDefault();
        var row_content = $(this).val();
        var row_content_json = JSON.parse(row_content);
        var url = '/password_details/' + row_content_json.id + "/"
        var d = $("<div class='warning'><iframe width='100%' height='100%' src="+ url + "></iframe></div>");
                d.dialog({
                        width : 400,
                        height : 400,
                        buttons: [
                            {
                              text: "Ok",
                              icons: {
                                primary: "ui-icon-heart"
                              },
                              click: function() {
                                $( this ).dialog( "destroy" );
                              }
                            }
                          ]
                    });


        e.preventDefault();

    });
});
