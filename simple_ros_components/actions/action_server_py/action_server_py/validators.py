from action_interfaces.action import AddUntil


def validate_goal(goal_request: AddUntil.Goal) -> tuple[bool, str]:
    if goal_request.target_number <= 0:
        return False, "Reject target number <= 0"

    if goal_request.target_number % 2 == 1:
        return False, f"Reject odd target number {goal_request.target_number}"

    if goal_request.period < 0 or goal_request.period > 10:
        return False, "Reject period not in [0,10]"

    return True, ""
